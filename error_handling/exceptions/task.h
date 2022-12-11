#include <string>
#include <memory>
#include <unordered_map>
#include <vector>

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
    virtual ObjPtr Evaluate(const Context& ctx) const = 0;
};

using ExprPtr = std::shared_ptr<const IExpression>;

class IStatement : public IObject {
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
    virtual ObjPtr Execute(Context& ctx) const = 0;
};

using StatementPtr = std::shared_ptr<const IStatement>;

struct Token {
    /*
     * Токен -- минимальная неделимая единица разбора исходной строки
     */
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
    std::string value_;
};

class StatementParser {
    /*
     * Класс, реализующий разбор (парсинг) входной строки в инструкцию для калькулятора
     */
public:
    StatementPtr ParseString(const std::string& s, const Context& ctx);

    /*
     * Разобрать исходную строку на токены, которые можно в дальнейшем преобразовать в инструкцию для калькулятора
     * Эта функция будет полезна в `ParseString`
     */
    std::vector<Token> Tokenize(const std::string& s, const Context& ctx);
};

class Calculator {
public:
    Calculator() {
        // Добавляем базовые предопределенные функции в калькулятор.
        // Реализовать классы нужно самостоятельно, тригонометрические функции брать из стандартной библиотеки
        ctx_["+"] = std::make_shared<AdditionFunction>();
        ctx_["-"] = std::make_shared<SubstractionFunction>();
        ctx_["*"] = std::make_shared<MultiplicationFunction>();
        ctx_["/"] = std::make_shared<DivisionFunction>();
        ctx_["sin"] = std::make_shared<SinusFunction>();
        ctx_["cos"] = std::make_shared<CosinusFunction>();
    }

    std::string ProcessString(const std::string& stringExpression) {
        StatementPtr statement = ParseLine(stringExpression);
        return ExecuteStatement(statement)->Stringify();
    }

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

    StatementPtr ParseLine(const std::string& line) {
        return parser_.ParseString(line, ctx_);
    }

    ExprPtr ParseExpressionLine(const std::string line) {
        /*
         * Разобрать строку в выражение.
         * Если строка задает инструкцию присваивания, а не инструкцию вычисления выражения, то бросить исключение
         */
        // ToDo: Реализовать самостоятельно
    }
private:
    Context ctx_;
    StatementParser parser_;
};
