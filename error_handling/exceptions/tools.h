#include "task.h"
#include <string>
#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>
#include <sstream>
#include <iostream>
#include "list"
#include "cmath"

using namespace std;

bool isDigit(const std::string &myString) {
//    std::cout << "digit test: " << myString << std::endl;
    std::istringstream iss(myString);
    float f;
    iss >> std::noskipws >> f; // noskipws considers leading whitespace invalid
    // Check the entire string was consumed and if either failbit or badbit is set
    return iss.eof() && !iss.fail();
}

class NumberObj : public IObject {
    const double num_;
public:
    explicit NumberObj(double num) : num_(num) {}

    std::string Stringify() const override {
        return std::to_string(num_);
    }

    double Raw() const {
        return num_;
    }
};

class LambdaObj : public IObject {
    std::vector<Token> tokens_;
    std::string strexpr_;
    const Context &ctx_;
public:
    LambdaObj(std::vector<Token> tokens, std::string strexpr, const Context& ctx) : tokens_(std::move(tokens)),
                                                                                    strexpr_(std::move(strexpr)),
                                                                                    ctx_(ctx) {
        cout << "lambdaObj constructor " << strexpr_ << endl;
        int i = 0;
        while (tokens_[i].type_ != Token::Type::Arrow) {
            i++;
        }
        i++;
        for (int j = i; j < tokens_.size(); ++j) {
            if (tokens_[j].type_ == Token::Type::Name && ctx_.find(tokens_[j].value_) != ctx_.end() &&
                tokens_[j].value_ != "+" && tokens_[j].value_ != "-" && tokens_[j].value_ != "*" && tokens_[j].value_ != "/" &&
                tokens_[j].value_ != "sin" && tokens_[j].value_ != "cos") {
                    bool isArgument = false;
                    for (int k = 0; k < i; ++k) {
                        if (tokens_[j].value_ == tokens_[k].value_) {
                            isArgument = true;
                            break;
                        }
                    }
                    if (!isArgument) {
                        tokens_[j].type_ = Token::Type::Literal; // TODO: мб встроить лямбду внутри нашей лямбды
                        tokens_[j].value_ = ctx_.at(tokens_[j].value_)->Stringify();
                    }
            }
        }

    }

    LambdaObj(const LambdaObj& obj) : tokens_(obj.tokens_), strexpr_(obj.strexpr_), ctx_(obj.ctx_) {}
    ExprPtr Do(const vector<Token> &args) const {
        vector<Token> tokens;
        tokens.emplace_back(Token::Type::LeftParenthesis, "(");
        for (auto it: tokens_) {
            tokens.push_back(std::move(it));
        }
        tokens.emplace_back(Token::Type::RightParenthesis, ")");
        for (auto it: args) {
            tokens.push_back(std::move(it));
        }
        return make_shared<LambdaExpression>(tokens, strexpr_);
    }
    const LambdaObj removeOneArgument() const {
        vector<Token> tokens = tokens_;
        int i = 0;
        while (tokens[i].type_ != Token::Type::Arrow) i++;
        i++;
        tokens.erase(tokens.begin() + i);
        if (tokens[++i].type_ == Token::Type::Name) {
            tokens.erase(tokens_.begin() + i);
        }
        i = 0;
        while (tokens[i].type_ != Token::Type::LambdaSign) i++;
        i++;
        tokens.erase(tokens.begin() + i);
        return {tokens, strexpr_, ctx_};
    }
    int getArgsCount() const {
        int i = 0;
        while (tokens_[i].type_ != Token::Type::LambdaSign) i++;
        while (tokens_[i].type_ != Token::Type::Arrow) i++;
        return i - 1;
    }
    static const LambdaObj makeUnaryLambda(){
        vector<Token> tokens;
        tokens.emplace_back(Token::Type::LambdaSign, "\\");
        tokens.emplace_back(Token::Type::Name, "x");
        tokens.emplace_back(Token::Type::Arrow, "->");
        tokens.emplace_back(Token::Type::Name, "x");
        return {tokens, "unary", Context()};
    }
    std::string Stringify() const override {
        return strexpr_;
    }
};


class LambdaCurryingObj : public IObject {
    std::vector<Token> tokens_;
    std::shared_ptr<const LambdaObj> lambdaObj_;
    const Context &ctx_;
public:
    LambdaCurryingObj(std::shared_ptr<const LambdaObj> lambdaObj, std::vector<Token> tokens, const Context& ctx):
        tokens_(std::move(tokens)), lambdaObj_(std::move(lambdaObj)), ctx_(ctx){}
    std::string Stringify() const override {
        return lambdaObj_->Stringify();
    }
    ExprPtr Do(const vector<Token> &args) const {
        vector<Token> tokens;
        for (auto it: tokens_) {
            tokens.push_back(std::move(it));
        }
        for (auto it: args) {
            tokens.push_back(std::move(it));
        }
        return lambdaObj_->Do(tokens);
    }
};


class AdditionFunction : public IObject {
public:
    AdditionFunction() = default;

    std::string Stringify() const override {
        return "+";
    }

    static NumberObj Do(double x, double y) {
        return NumberObj(x + y);
    }
};

class SubstractionFunction : public IObject {
public:
    std::string Stringify() const override {
        return "-";
    }

    static NumberObj Do(double x, double y) {
        return NumberObj(x - y);
    }
};

class MultiplicationFunction : public IObject {
public:
    std::string Stringify() const override {
        return "*";
    }

    static NumberObj Do(double x, double y) {
        return NumberObj(x * y);
    }
};

class DivisionFunction : public IObject {
public:
    std::string Stringify() const override {
        return "/";
    }

    static NumberObj Do(double x, double y) {
        return NumberObj(x / y);
    }
};

class SinusFunction : public IObject {
public:
    std::string Stringify() const override {
        return "sin";
    }

    static NumberObj Do(double x) {
        return NumberObj(sin(x));
    }
};

class CosinusFunction : public IObject {
public:
    std::string Stringify() const override {
        return "cos";
    }

    static NumberObj Do(double x) {
        return NumberObj(cos(x));
    }
};

struct CombinatoryLogic {
    static const std::string Combinator;
    static const std::string Distributor;
    static const std::string Deleter;
};
const std::string CombinatoryLogic::Combinator = "I";
const std::string CombinatoryLogic::Distributor = "S";
const std::string CombinatoryLogic::Deleter = "K";


std::vector<Token> StatementParser::Tokenize(const std::string &s, const Context &ctx) {
    std::vector<Token> tokens;
    if (s.find('=') != std::string::npos) {
        strtype_ = StringType::Assigment;
    } else strtype_ = StringType::Expression;
    std::string scopy = s + " ";
    size_t pos;
    while ((pos = scopy.find(' ')) != std::string::npos) {
        std::string sub = scopy.substr(0, pos);
        if (isDigit(sub)) {
            tokens.emplace_back(Token::Type::Literal, sub);
        } else if (sub == "(") {
            tokens.emplace_back(Token::Type::LeftParenthesis, sub);
        } else if (sub == ")") {
            tokens.emplace_back(Token::Type::RightParenthesis, sub);
        } else if (sub == "\\") {
            tokens.emplace_back(Token::Type::LambdaSign, sub);
        } else if (sub == "->") {
            tokens.emplace_back(Token::Type::Arrow, sub);
        } else if (sub == "=") {
            tokens.emplace_back(Token::Type::EqualsSign, sub);
        } else {
            // todo: тут доделать эксепшены или хз что-то
            if (sub != "") {
                tokens.emplace_back(Token::Type::Name, sub);
            }
        }
        scopy.erase(0, pos + 1);
    }
    return tokens;
}

void ExecuteOperation(std::vector<double> &values, std::vector<std::string> &operators) {
    std::string op = *(operators.end() - 1);
    operators.erase(operators.end() - 1);
    if (op != "sin" && op != "cos" && op != "+" && op != "-" && op != "*" && op != "/" && !isDigit(op)) {
        throw UnresolvedReferenceException("Unknown name " + op);
    }
    double rightOperand = *(values.end() - 1);
    values.erase(values.end() - 1);
    double leftOperand;
    if (op != "sin" && op != "cos") {
        leftOperand = *(values.end() - 1);
        values.erase(values.end() - 1);
    }

    if (op == "+") {
        values.push_back(AdditionFunction::Do(leftOperand, rightOperand).Raw());
    } else if (op == "-") {
        values.push_back(SubstractionFunction::Do(leftOperand, rightOperand).Raw());
    } else if (op == "*") {
        values.push_back(MultiplicationFunction::Do(leftOperand, rightOperand).Raw());
    } else if (op == "/") {
        values.push_back(DivisionFunction::Do(leftOperand, rightOperand).Raw());
    } else if (op == "sin") {
        values.push_back(SinusFunction::Do(rightOperand).Raw());
    } else if (op == "cos") {
        values.push_back(CosinusFunction::Do(rightOperand).Raw());
    }
}

void ProcessClosingParenthesis(std::vector<double> &values, std::vector<std::string> &operators) {
    while (operators[operators.size() - 1] != "(") {
        ExecuteOperation(values, operators);
    }
    operators.erase(operators.end() - 1);
}

int ProcessInputNumber(std::vector<Token> &expr, int n, int pos, std::vector<double> &values) {
    values.push_back(std::stod(expr[pos].value_));
    return ++pos;
}

bool OperatorCausesEvaluation(const std::string &op, const std::string &prevOp) {
    if (op == "+" && prevOp != "(") {
        return true;
    } else if (op == "-" && prevOp != "(") {
        return true;
    } else if (op == "*" && (prevOp == "*" || prevOp == "/" || prevOp == "cos" || prevOp == "sin")) {
        return true;
    } else if (op == "/" && (prevOp == "*" || prevOp == "/" || prevOp == "cos" || prevOp == "sin")) {
        return true;
    } else if (op == ")") return true;
    return false;
}

void ProcessInputOperator(std::string op, std::vector<double> &values, std::vector<std::string> &operators) {
    while (operators.size() != 0 && OperatorCausesEvaluation(op, *(operators.end() - 1))) {
        ExecuteOperation(values, operators);
    }
    operators.push_back(op);
}

double ExprEvaluate(std::vector<Token> expr) {
    std::vector<double> values;
    std::vector<std::string> operators;
    operators.emplace_back("(");

    int pos = 0;
    int n = expr.size();

    while (pos <= n) {
        if (pos == n || expr[pos].value_ == ")") {
            ProcessClosingParenthesis(values, operators);
            pos++;
        } else if (isDigit(expr[pos].value_)) {
            pos = ProcessInputNumber(expr, n, pos, values);
        } else {
            ProcessInputOperator(expr[pos].value_, values, operators);
            pos++;
        }
    }
    return values[values.size() - 1]; // мб erase
}

Calculator::Calculator() {
    ctx_["+"] = std::make_shared<AdditionFunction>();
    ctx_["-"] = std::make_shared<SubstractionFunction>();
    ctx_["*"] = std::make_shared<MultiplicationFunction>();
    ctx_["/"] = std::make_shared<DivisionFunction>();
    ctx_["sin"] = std::make_shared<SinusFunction>();
    ctx_["cos"] = std::make_shared<CosinusFunction>();
}

class SimplyExpression : public IExpression {
    std::string strexpr_;
    vector<Token> tokens_;
public:
    SimplyExpression(vector<Token> tokens, std::string strexpr) : strexpr_(std::move(strexpr)),
                                                                  tokens_(std::move(tokens)) {
        cout << "expr constructor " << strexpr_ << endl;
    }

    ObjPtr Evaluate(const Context &ctx) const override {
        return make_shared<const NumberObj>(ExprEvaluate(tokens_));
    }

    std::string Stringify() const override {
        cout << "stringinfy" << endl;
        return strexpr_;
    }
};

ExprPtr Calculator::ParseExpressionLine(const std::string line) {
    StatementPtr ptr3 = parser_.ParseString(line, ctx_);
    ObjPtr ptr2 = ptr3->Execute(ctx_);
    ExprPtr ptr = std::dynamic_pointer_cast<const IExpression>(ptr2);
    return ptr;
}


std::string Calculator::ProcessString(const std::string &stringExpression) {
    StatementPtr statement = ParseLine(stringExpression);
    return dynamic_pointer_cast<const IExpression>(statement->Execute(ctx_))->Evaluate(ctx_)->Stringify();
}
