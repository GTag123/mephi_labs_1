#include "tools.h"


LambdaExpression::LambdaExpression(vector<Token> tokens, std::string strexpr) : strexpr_(std::move(strexpr)),
                                                                                tokens_(std::move(tokens)) {
    cout << "lambda constructor " << strexpr_ << endl;
    parse_();
    for (int i = 0; i < (int) argvars_.size(); i++) {
        argmap_[argvars_[i].value_] = argtokens_[i].value_;
    }
}

void LambdaExpression::parse_() {
    for (int i = 0; i < (int) tokens_.size(); ++i) {
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

            while (j < (int) tokens_.size()) {
                argtokens.push_back(tokens_[j]);
                j++;
            }
            cout << "argtokens: " << endl;
            for (auto &i: argtokens) {
                cout << i.value_ << " ";
            }
            cout << endl;
            argtokens_ = std::move(argtokens);
            argvars_ = std::move(argvars);
            functokens_ = std::move(functokens);
            break;
        }
    }
}

ObjPtr LambdaExpression::Evaluate(const Context &ctx) const {
    vector<Token> functokenscopy_ = functokens_;
    for (int i = 0; i < (int) functokenscopy_.size(); ++i) {
        if (functokenscopy_[i].type_ == Token::Type::Name &&
            argmap_.find(functokenscopy_[i].value_) != argmap_.end()) {
            functokenscopy_[i].type_ = Token::Type::Literal;
            functokenscopy_[i].value_ = argmap_.at(functokens_[i].value_);
        } else {
            // TODO: добавить else если в мэйне есть такие проверки
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

class ExpressionStatement : public IStatement {
private:
    std::string expr_;
    vector<Token> tokens_;
public:
    ExpressionStatement(std::string expr, vector<Token> tokens) : expr_(std::move(expr)), tokens_(std::move(tokens)) {};

    ObjPtr Execute(Context &ctx) const override {
        vector<Token> tokenscopy = tokens_;

        if (std::find_if(tokens_.begin(), tokens_.end(),
                         [](const Token &t) { return t.type_ == Token::Type::LambdaSign; }) != tokens_.end()) {
            return make_shared<const LambdaExpression>(tokens_, expr_);
        } // если лямбда
        if (std::find_if(tokens_.begin(), tokens_.end(),
                         [](const Token &t) { return t.type_ == Token::Type::Name; }) != tokens_.end()) {
            for (int i = 0; i < (int) tokenscopy.size(); ++i) {
                if (tokenscopy[i].type_ == Token::Type::Name) {
                    if (ctx.find(tokenscopy[i].value_) != ctx.end()) {
                        if (std::shared_ptr<const LambdaObj> obj = dynamic_pointer_cast<const LambdaObj>(
                                ctx[tokenscopy[i].value_])) { // чекаем если вызов функции
                            // теперь нужно проверить не композиция ли? для этого смотрим на второй токен
                            if (tokenscopy.size() > i + 2 && tokenscopy[i + 1].type_ == Token::Type::Name &&
                                tokenscopy[i + 2].type_ == Token::Type::Name) {
                                int j = i + 1;
                                while (j < (int) tokenscopy.size() && tokenscopy[j].type_ == Token::Type::Name) {
                                    j++;
                                }
                                int argsind = j--;
                                vector<Token> argtokens;
                                for (int k = argsind; k < (int) tokenscopy.size(); ++k) {
                                    argtokens.push_back(tokenscopy[k]);
                                }
                                ObjPtr res;
                                if (std::shared_ptr<const LambdaObj> subfunc = dynamic_pointer_cast<const LambdaObj>(
                                        ctx[tokenscopy[j].value_])) {
                                    res = subfunc->Do(argtokens)->Evaluate(ctx);
                                }
                                j--;
                                while (j > 1) {
                                    if (std::shared_ptr<const LambdaObj> subfunc = dynamic_pointer_cast<const LambdaObj>(
                                            ctx[tokenscopy[j].value_])) {
                                        argtokens.clear();
                                        argtokens.emplace_back(Token::Type::Literal, res->Stringify());
                                        res = subfunc->Do(argtokens)->Evaluate(ctx);
                                    } else {
                                        return nullptr; // ошибка каста
                                    }
                                    j--;
                                }
                                if (std::shared_ptr<const LambdaObj> subfunc = dynamic_pointer_cast<const LambdaObj>(
                                        ctx[tokenscopy[j].value_])) {
                                    argtokens.clear();
                                    argtokens.emplace_back(Token::Type::Literal, res->Stringify());
                                    return subfunc->Do(argtokens);
                                } else {
                                    return nullptr; // ошибка каста
                                }
                            } else if (tokenscopy.size() > i + 1 && tokenscopy[i + 1].type_ == Token::Type::Name) {
                                int j = i + 1;
                                while (j < (int) tokenscopy.size()) {
                                    if (std::shared_ptr<const NumberObj> obj = dynamic_pointer_cast<const NumberObj>(
                                            ctx[tokenscopy[j].value_])) {
                                        tokenscopy[j].type_ = Token::Type::Literal;
                                        tokenscopy[j].value_ = obj->Stringify();
                                    }
                                    j++;
                                }
                                vector<Token> argtokens;
                                for (j = i + 1; j < (int) tokenscopy.size(); ++j) {
                                    argtokens.push_back(tokenscopy[j]);
                                }
                                return obj->Do(argtokens);
                            } else {
                                vector<Token> argtokens;
                                for (int j = i + 1; j < (int) tokenscopy.size(); ++j) {
                                    argtokens.push_back(tokenscopy[j]);
                                }
                                return obj->Do(argtokens);
                            }
                        } else if (std::shared_ptr<const LambdaCurryingObj> obj = dynamic_pointer_cast<const LambdaCurryingObj>(
                                ctx[tokenscopy[i].value_])) {
                            vector<Token> argtokens;
                            for (int j = i + 1; j < (int) tokenscopy.size(); ++j) {
                                argtokens.push_back(tokenscopy[j]);
                            }
                            return obj->Do(argtokens);
                        } else {
                            tokenscopy[i].type_ = Token::Type::Literal;
                            tokenscopy[i].value_ = ctx[tokenscopy[i].value_]->Stringify();
                        }
                    }
                }
            }
        }
        return std::make_shared<const SimplyExpression>(tokenscopy, expr_);
    }

    std::string Stringify() const override {
        std::cout << "------------ExprStatement stringify: " << expr_ << " -----------------" << std::endl;
        return expr_;
    }
};

class AssignmentExpression : public IExpression {
    ObjPtr Evaluate(const Context &ctx) const override {
        return make_shared<NumberObj>(0);
    }

    std::string Stringify() const override {
        return "null assignmentExpr";
    }
};

class AssignmentStatement : public IStatement {
private:
    std::string expr_;
    vector<Token> tokens_;
public:
    AssignmentStatement(std::string expr, vector<Token> tokens) : expr_(std::move(expr)), tokens_(std::move(tokens)) {};

    ObjPtr Execute(Context &ctx) const override {
        std::string name = tokens_[0].value_;
        vector<Token> tokenscopy(tokens_.begin() + 2, tokens_.end());
        if (tokenscopy.size() == 1 && tokenscopy[0].type_ == Token::Type::Literal) {
            ctx[name] = SimplyExpression(tokenscopy, expr_).Evaluate(ctx);
        } else if (tokenscopy[0].type_ == Token::Type::LambdaSign) {
            ctx[name] = make_shared<const LambdaObj>(tokenscopy, expr_, ctx);
        } else if (tokenscopy[0].type_ == Token::Type::Name && ctx.find(tokenscopy[0].value_) != ctx.end()) {
            std::shared_ptr<const LambdaObj> func = dynamic_pointer_cast<const LambdaObj>(ctx[tokenscopy[0].value_]);
            if (func && (tokenscopy[0].value_ == CombinatoryLogic::Distributor ||
                         tokenscopy[0].value_ == CombinatoryLogic::Deleter ||
                         tokenscopy[0].value_ == CombinatoryLogic::Combinator)) {
                for (const auto &it: tokenscopy) {
                    if (it.value_ != CombinatoryLogic::Distributor && it.value_ != CombinatoryLogic::Deleter &&
                        it.value_ != CombinatoryLogic::Combinator) {
                        return nullptr;
                    }
                }
                // logic:
                if (tokenscopy[0].value_ == CombinatoryLogic::Distributor) {
                    int j = 1;
                    if (std::shared_ptr<const LambdaObj> subfunc = dynamic_pointer_cast<const LambdaObj>(
                            ctx[CombinatoryLogic::Distributor])) {
                        int argsCount = subfunc->getArgsCount();
                        while (j < (int) tokenscopy.size()) {
                            if (tokenscopy[j].value_ == CombinatoryLogic::Deleter) {
                                argsCount--;
                            }
                            j++;
                        }
                        if (argsCount == 1) {
                            ctx[name] = make_shared<const LambdaObj>(LambdaObj::makeUnaryLambda());
                        } else if (argsCount > 1) { // todo composition }
                        }
                    }
                }
            }
            else if (func) {
                ctx[name] = make_shared<const LambdaCurryingObj>(func, vector<Token>(tokenscopy.begin() + 1,
                                                                                     tokenscopy.end()), ctx);
            }
        } // else {}
        return make_shared<AssignmentExpression>();
    }

    std::string Stringify() const override {
        std::cout << "------------AssignmentStatement stringify: " << expr_ << " -----------------" << std::endl;
        return expr_;
    }
};

StatementPtr StatementParser::ParseString(const std::string &s, const Context &ctx) {
    auto tokens = Tokenize(s, ctx);
    if (std::find_if(tokens.begin(), tokens.end(),
                     [](const Token &t) { return t.type_ == Token::Type::EqualsSign; }) != tokens.end()) {
        cout << "assigment execute" << endl;
        return std::make_shared<AssignmentStatement>(s, tokens);
    } else {
        return std::make_shared<ExpressionStatement>(s, tokens);
    }
}




