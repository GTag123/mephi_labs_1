#include <string>
#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>
#include <sstream>
#include <iostream>
#include "list"
#include "task.h"
#include "cmath"

using namespace std;

bool isDigit(const std::string& myString) {
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

    [[nodiscard]] std::string Stringify() const override {
        return std::to_string(num_);
    }

    [[nodiscard]] double Raw() const {
        return num_;
    }
};

class AdditionFunction : public IObject {
public:
    AdditionFunction() = default;

    [[nodiscard]] std::string Stringify() const override {
        return "+";
    }

    static NumberObj Do(double x, double y) {
        return NumberObj(x + y);
    }
};

class SubstractionFunction : public IObject {
public:
    [[nodiscard]] std::string Stringify() const override {
        return "-";
    }

    static NumberObj Do(double x, double y) {
        return NumberObj(x - y);
    }
};

class MultiplicationFunction : public IObject {
public:
    [[nodiscard]] std::string Stringify() const override {
        return "*";
    }

    static NumberObj Do(double x, double y) {
        return NumberObj(x * y);
    }
};

class DivisionFunction : public IObject {
public:
    [[nodiscard]] std::string Stringify() const override {
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
            tokens.emplace_back(Token::Type::Name, sub);
        }
        scopy.erase(0, pos + 1);
    }
    return tokens;
}

void ExecuteOperation(std::vector<double> &values, std::vector<std::string> &operators) {
    std::string op = *(operators.end() - 1);
    operators.erase(operators.end() - 1);
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
    bool isFunc = false;
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

struct CtxTypes: public IObject{
    enum class Type{
        VAR,
        LAMBDA
    };
    CtxTypes(Type type, vector<Token> value): type_(type), value_(std::move(value)){}
    Type type_;
    vector<Token> value_;
    std::string Stringify() const override{
        string s; for (auto i: value_) s += i.value_ + " ";
        return s;
    }
};

class SimplyExpression : public IExpression {
    std::string strexpr_;
    vector<Token> tokens_;
public:
    SimplyExpression(vector<Token> tokens, std::string strexpr) : tokens_(std::move(tokens)),
                                                                  strexpr_(std::move(strexpr)) {
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

class LambdaExpression : public IExpression {
    std::string strexpr_;
    vector<Token> functokens_; // тело функции
    vector<Token> argvars_; // имена аргументов
    vector<Token> argtokens_; // аргументы, которые передаются в функцию
    unordered_map<std::string, std::string> argmap_;
public:
    // парсинг на лямбду и аргументы в statementе
    LambdaExpression(vector<Token> func, vector<Token> argvars, vector<Token> argtokens, std::string strexpr) :
            functokens_(std::move(func)), argvars_(std::move(argvars)), argtokens_(std::move(argtokens)),
            strexpr_(std::move(strexpr)) {
        cout << "lambda constructor " << strexpr_ << endl;
        for (int i = 0; i < argvars_.size(); i++) {
            argmap_[argvars_[i].value_] = argtokens_[i].value_;
        }
    }

    ObjPtr Evaluate(const Context &ctx) const override {
        vector<Token> functokenscopy_ = functokens_;
        for (int i = 0; i < functokenscopy_.size() - 1; ++i) {
            if (functokenscopy_[i].type_ == Token::Type::Name) { // в этих ифах всё поломается
                if (functokenscopy_[i + 1].type_ == Token::Type::Name &&
                    argmap_.find(functokenscopy_[i + 1].value_) != argmap_.end()) {
                    functokenscopy_[i + 1].type_ = Token::Type::Literal;
                    functokenscopy_[i + 1].value_ = argmap_.at(functokens_[i + 1].value_);
                } else {
                    // TODO: добавить else если в мэйне есть такие проверки
                }
            }
        }
        cout << endl;
        cout << "Labmda tokens value: " << endl;
        for (auto &i: functokenscopy_) {
            cout << i.value_ << " ";
        }
        cout << endl;
        auto t2 = SimplyExpression(functokenscopy_, strexpr_);
        auto t1 = t2.Evaluate(ctx);
        std::shared_ptr<const NumberObj> expr = dynamic_pointer_cast<const NumberObj>(t1);
        return expr;
    }

    std::string Stringify() const override {
        return strexpr_;
    }
};

class AssigmentStatement : public IStatement{
    std::string expr_;
    vector<Token> tokens_;
public:
    AssigmentStatement(std::string expr, vector<Token> tokens) : expr_(std::move(expr)), tokens_(std::move(tokens)) {};

    ObjPtr Execute(Context &ctx) const override{
        for (int i = 0; i < tokens_.size(); ++i) {
            while (tokens_[++i].value_ != "="){}
            if (tokens_[i+1].value_ != "\\"){ // TODO: обработать уже объявленные names в lambda
                vector<Token> lambda;
                for (int j = i+1; j < tokens_.size(); ++j) {
                    lambda.push_back(tokens_[j]);
                }
                ctx[tokens_[i-1].value_] = make_shared<CtxTypes>(CtxTypes(CtxTypes::Type::LAMBDA, ));
            } else {
                ctx[tokens_[i-1].value_] = make_shared<CtxTypes>(
                        CtxTypes(CtxTypes::Type::VAR, {{Token::Type::Literal, tokens_[i+1].value_}}));
            }
        }
    }
    std::string Stringify() const override {
        std::cout << "------------AssigStatement stringify: " << expr_ << " -----------------" << std::endl;
        return expr_;
    }
};
class ExpressionStatement : public IStatement {
private:
    std::string expr_;
    vector<Token> tokens_;
public:
    ExpressionStatement(std::string expr, vector<Token> tokens) : expr_(std::move(expr)), tokens_(std::move(tokens)) {};

    ObjPtr Execute(Context &ctx) const override {
        for (int i = 0; i < tokens_.size(); ++i) {
            if (tokens_[i].type_ == Token::Type::LambdaSign) {
                vector<Token> argvars;
                vector<Token> argtokens;
                vector<Token> functokens;
                int j = i + 1;
                while (tokens_[j].type_ != Token::Type::Arrow) {
                    argvars.push_back(tokens_[j]);
                    j++;
                }
                cout << "argvars: " << endl;
                for (auto &i: argvars) {
                    cout << i.value_ << " ";
                }
                cout << endl;
                j++;

                while (tokens_[j].type_ != Token::Type::RightParenthesis) {
                    functokens.push_back(tokens_[j]);
                    j++;
                }
                cout << "functokens: " << endl;
                for (auto &i: functokens) {
                    cout << i.value_ << " ";
                }
                cout << endl;
                j++;

                while (j < tokens_.size()) {
                    argtokens.push_back(tokens_[j]);
                    j++;
                }
                cout << "argtokens: " << endl;
                for (auto &i: argtokens) {
                    cout << i.value_ << " ";
                }
                cout << endl;

                return make_shared<const LambdaExpression>(functokens, argvars, argtokens, expr_);
            }

        }

        return std::make_shared<const SimplyExpression>(tokens_, expr_);
    }

    std::string Stringify() const override {
        std::cout << "------------ExprStatement stringify: " << expr_ << " -----------------" << std::endl;
        return expr_;
    }
};

StatementPtr StatementParser::ParseString(const std::string &s, const Context &ctx) {
    StatementPtr statementPtr = std::dynamic_pointer_cast<const IStatement>(make_shared<const ExpressionStatement>(s, Tokenize(s, ctx)));
    return statementPtr;
}

ExprPtr Calculator::ParseExpressionLine(const std::string& line) {
    StatementPtr ptr3 = parser_.ParseString(line, ctx_);
    ObjPtr ptr2 = ptr3->Execute(ctx_);
    ExprPtr ptr = std::dynamic_pointer_cast<const IExpression>(ptr2);
    return ptr;
}

std::string Calculator::ProcessString(const std::string &stringExpression) {
    StatementPtr statement = ParseLine(stringExpression);
    return dynamic_pointer_cast<const IExpression>(statement->Execute(ctx_))->Evaluate(ctx_)->Stringify();
}

Calculator::Calculator() {
    ctx_["+"] = std::make_shared<AdditionFunction>();
    ctx_["-"] = std::make_shared<SubstractionFunction>();
    ctx_["*"] = std::make_shared<MultiplicationFunction>();
    ctx_["/"] = std::make_shared<DivisionFunction>();
    ctx_["sin"] = std::make_shared<SinusFunction>();
    ctx_["cos"] = std::make_shared<CosinusFunction>();
}