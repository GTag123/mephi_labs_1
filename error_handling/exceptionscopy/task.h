#include <string>
#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>
#include <sstream>
#include <iostream>
#include "list"
#include "calcexpr.h"

class IObject {
    /*
     * Базовый класс для всех объектов, с которыми работает калькулятор
     */
public:
    virtual ~IObject() = default;

    /*
     * Преобразование объекта в строку. Удобно, когда нужно вывести объект в описание исключения или в логи.
     * В проверяющем коде задания ожидается, что объекты, представляющие числовые литералы, будут приводиться к строке
     * так, как это делает функция std::to_string (не стоит изобретать свой велосипед в этом месте)
     */
    virtual std::string Stringify() const = 0;
};

using ObjPtr = std::shared_ptr<const IObject>;
using Context = std::unordered_map<std::string, ObjPtr>;

class IExpression : public IObject {
    /*
     * Базовый класс для вычислимых выражений, таких как
     * Числовые константы, функции (в т.ч. лямбда-функции), имена, операции применения функции к аргументу
     */
public:
    /*
     * Вычисляет значение выражения. Результатом вычисления может быть как константа, так и какой-то более сложный объект
     * Например, функция после применения её в какому-то аргументу может вернуть другую функцию.
     *
     * Подразумевается, что функции в данной задаче каррированные, т.е. могут быть применены только к одному аргументу
     * за один раз. В таком случае функция с несколькими аргументами превращается в функцию от одного аргумента, которая возвращает
     * другую функцию, имеющую на один аргумент меньше. См. https://ru.wikipedia.org/wiki/Каррирование
     */
    virtual ObjPtr Evaluate(const Context &ctx) const = 0;
};

using ExprPtr = std::shared_ptr<const IExpression>;
//class NumberObj : public IObject {
//    double res_;
//public:
//    NumberObj(double res) : res_(res) {}
//
//    std::string Stringify() const override {
//        return std::to_string(res_);
//    }
//};

//class SimplyExpression : public IExpression {
//    std::string strexpr_;
//    int res;
//public:
//    SimplyExpression(std::string expr) : strexpr_(std::move(expr)) {};
//
//    ObjPtr Evaluate(const Context &ctx) const override {
//        RDCalculator calc(strexpr_);
//        return make_shared<NumberObj>(calc.getExpressionResult());
//    }
//    std::string Stringify() const override {
//        return strexpr_;
//    }
//};




class IStatement : public IObject {
    // универсальная хуйня, execute по идее должен возвращать iexpression
    // тут выбирается либо вычисление, которое делается полностью в iexpression,
    // либо обычное создание/присвоение переменной, которое ПОХОДУ нужно хранить в контексте
    /*
     * Базовый класс для инструкций, подаваемых на вход калькулятору
     * Одна входная строка в итоге должна быть преобразована в одну инструкцию
     * Инструкции бывают двух типов -- вычисление значения выражения и присваивание
     * Инструкция присваивания выглядит как `[name] = [expression]`, где
     *  `[name]` -- какое-то имя из букв и цифр, начинающееся с буквы;
     *  `[expression]` -- выражение, значение которого нужно запомнить под именем `[name]`.
     * Если в исходной строке не было знака равенства (который означает операцию присваивания),
     * то вся строка воспринимается как инструкция вычисления выражения
     */
public:
    virtual ObjPtr Execute(Context &ctx) const = 0;
};


using StatementPtr = std::shared_ptr<const IStatement>;

class Token {
    /*
     * Токен -- минимальная неделимая единица разбора исходной строки
     */
public:
    enum class Type {
        Literal = 0,  // литерал -- буквальное значение (числовая константа в данной задаче типа `1`, `25.48` и т.п.)
        Name,  // Какое-то имя из букв и цифр, начинающееся с буквы, либо знак встроенной бинарной операции (+-*/). Имя ссылается на какое-то значение (константа, функция)
        LeftParenthesis,  // Открывающая круглая скобка `(`
        RightParenthesis,  // Закрывающая круглая скобка скобка `)`
        LambdaSign,  // Условный символ "лямбда" `\` (в данном случае это левый слэш как в языке Haskell)
        Arrow,  // Стрелка `->`
        EqualsSign,  // Знак равенства `=`
    };

    Type type_;
//    ObjPtr func_;
    std::string value_;

    Token(Type type, std::string value) : type_(type), value_(value) {}
//    Token(Type type, std::string value, ObjPtr): type_(type), value_(value), func_(std::move(ObjPtr)) {}
};

//class ExpressionStatement : public IStatement {
//private:
//    std::string expr_;
//public:
//    ExpressionStatement(std::string expr) : expr_(expr){};
//    ObjPtr Execute(Context &ctx) const override {
//        return std::make_shared<SimplyExpression>(expr_)->Evaluate(ctx);
//    }
//    std::string Stringify() const override {
//        std::cout << "------------ExprStatement stringify: " << expr_ << " -----------------" << std::endl;
//        return expr_;
//    }
//
//};

class StatementParser {
    /*
     * Класс, реализующий разбор (парсинг) входной строки в инструкцию для калькулятора
     */
public:
    enum class StringType {
        Assigment = 1,
        Expression = 0
    };
    StringType strtype_;

    StatementPtr ParseString(const std::string &s, const Context &ctx);
//    {
////        std::vector<Token> tokens = Tokenize(s, ctx);
//        return make_shared<ExpressionStatement>(s);
//        // TODO: проходимся по всем токенам и выбираем нужный statement либо expression
//    }

    /*
     * Разобрать исходную строку на токены, которые можно в дальнейшем преобразовать в инструкцию для калькулятора
     * Эта функция будет полезна в `ParseString`
     */
    std::vector<Token> Tokenize(const std::string &s, const Context &ctx);
};

//class AssigmentStatement : public IStatement {
//    ObjPtr Execute(Context &ctx) const override {
//        // TODO
//    }
//};



//class SimplyExpression : public IExpression {
//private:
//    std::list<Token> tokens_;
//
//    std::pair<std::list<Token>::iterator, std::string>
//    tokenfind(const std::vector<std::string> &vecstr, std::list<Token> tokenscopy,
//              std::list<Token>::iterator startpos) const {
//        for (auto i = startpos; i != tokenscopy.end(); i++) {
//            for (auto j: vecstr) {
//                if (i->value_ == j) return {i, j};
//            }
//        }
//        return {tokenscopy.end(), ""};
//    }
//
//public:
//    explicit SimplyExpression(std::list<Token> tokens) : tokens_(std::move(tokens)) {};
//
//    ObjPtr Evaluate(const Context &ctx) const override {
//        std::list<Token> tokenscopy = tokens_; // говнокод, убираем токены пишем стрингами
//
//
//
//
//        std::pair<std::list<Token>::iterator, std::string> sign; // деление и умножения
//        while ((sign = this->tokenfind({"*", "\\"}, tokenscopy, tokenscopy.begin())).first != tokenscopy.end()) {
//            if (sign.second == "*") {
//                auto prev = (--sign.first)++;
//                auto next = (++sign.first)--;
//                sign.first->type_ = Token::Type::Literal;
//                sign.first->value_ = std::to_string(std::stoi(prev->value_) * std::stoi(
//                        next->value_)); // TODO: вместо примитивного умножения заменить на контекстные функции
//                tokenscopy.erase(prev);
//                tokenscopy.erase(next);
//            }
//        }
//
//
//
//
//    }
//
//    std::string Stringify() const override {
//
//    }
//
//};
//



//class AdditionFunction {
//    AdditionFunction() = default;
//
//    // вместа инта обжекты или другая хуйня
//    int MyExecute(int a, int b) const;
//};

class Calculator {
public:
    Calculator();
    // Добавляем базовые предопределенные функции в калькулятор.
    // Реализовать классы нужно самостоятельно, тригонометрические функции брать из стандартной библиотеки
    // TODO: хуй знает как вызвать функцию вычисления в классах функций
    // мб dynamic castом
    std::string ProcessString(const std::string &stringExpression);

    ObjPtr ExecuteStatement(StatementPtr statement) {
        /*
         * Выполнить данную инструкцию.
         * В результате выполнения инструкции присваивания будут происходить изменения в контексте
         * В результате выполнения инструкции вычисления выражения контекст не должен меняться
         */
        return statement->Execute(ctx_);
    }

    ObjPtr EvaluateExpression(ExprPtr expression) const {
        return expression->Evaluate(ctx_);
    }

    StatementPtr ParseLine(const std::string &line) {
        return parser_.ParseString(line, ctx_);
    }

    ExprPtr ParseExpressionLine(const std::string& line);

        /*
         * Разобрать строку в выражение.
         * Если строка задает инструкцию присваивания, а не инструкцию вычисления выражения, то бросить исключение
         */
        // ToDo: Реализовать самостоятельно

private:
    Context ctx_;
    StatementParser parser_;
};
