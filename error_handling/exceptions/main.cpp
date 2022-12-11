#include "task.h"
#include <iostream>
#include <cassert>

using namespace std;

void TestSimpleMath() {
    Calculator calc;

    ExprPtr expr = calc.ParseExpressionLine("5 + 8");
    cout << expr->Stringify() << endl;
    cout << calc.EvaluateExpression(expr)->Stringify() << endl;

    assert(calc.EvaluateExpression(expr)->Stringify() == "13.000000");
    assert(calc.ProcessString("5 + 10 * 8 - 10") == "75.000000");
    assert(calc.ProcessString("10") == "10.000000");
    assert(calc.ProcessString("( 10 )") == "10.000000");
    assert(calc.ProcessString("( 5 * 2 )") == "10.000000");
    assert(calc.ProcessString("100 / ( 5 * 2 ) * 5") == "50.000000");
}

void TestUnaryFunctions() {
    Calculator calc;

    ExprPtr expr = calc.ParseExpressionLine("sin 3.1415962");
    cout << expr->Stringify() << endl;
    cout << calc.EvaluateExpression(expr)->Stringify() << endl;

    assert(abs(strtod(calc.EvaluateExpression(expr)->Stringify().c_str(), nullptr)) < 0.00001);
    assert(abs(strtod(calc.ProcessString("sin ( 3.1415962 / 2 )").c_str(), nullptr) - 1) < 0.00001);
    assert(abs(strtod(calc.ProcessString("sin 3.1415962 + 2").c_str(), nullptr) - 2) < 0.00001);
}

void TestLambda() {
    Calculator calc;

    ExprPtr expr = calc.ParseExpressionLine(R"(( \ a b -> sin a * sin a + cos b * cos b ) 100500 100500)");
    cout << expr->Stringify() << endl;
    cout << calc.EvaluateExpression(expr)->Stringify() << endl;

    assert(abs(strtod(calc.EvaluateExpression(expr)->Stringify().c_str(), nullptr) - 1) < 0.00001);
    assert(abs(strtod(calc.ProcessString(R"(( \ x -> cos x ) 0)").c_str(), nullptr) - 1) < 0.00001);
}

void TestAssignment() {
    Calculator calc;

    calc.ProcessString("a = 5");
    assert(calc.ProcessString("a") == "5.000000");
    calc.ProcessString("a = 55");
    assert(calc.ProcessString("a") == "55.000000");
    calc.ProcessString("b = 20");
    assert(calc.ProcessString("a + b") == "75.000000");
    calc.ProcessString(R"(foo = \ x -> x + a - b )");
    assert(calc.ProcessString("foo 10") == "45.000000");

    // Лямбда сохраняет свой контекст в момент создания. Присваивание новых значений старым именам не влияет на вычисления
    calc.ProcessString("b = 100");
    assert(calc.ProcessString("foo 10") == "45.000000");

    // Аргументы скрывают внешние имена внутри лямбды, но не затирает их во внешнем контексте
    calc.ProcessString(R"(bar = \ a b -> a + b)");
    assert(calc.ProcessString("bar 10 10") == "20.000000");
    assert(calc.ProcessString("a + b") == "155.000000");

    // Для интересующихся, что тут такое происходит: https://ru.wikipedia.org/wiki/Композиция_функций
    calc.ProcessString(R"(compose = \ f g x -> f ( g x ))");
    calc.ProcessString(R"(plus1 = \ x -> x + 1)");
    calc.ProcessString(R"(mult5 = \ x -> x * 5)");
    assert(calc.ProcessString("compose plus1 mult5 1") == "6.000000");
    assert(calc.ProcessString("compose mult5 plus1 1") == "10.000000");
}

void TestCurrying() {
    /*
     * Подразумевается, что функции в данной задаче каррированные, т.е. могут быть применены только к одному аргументу
     * за один раз. В таком случае функция с несколькими аргументами превращается в функцию от одного аргумента, которая возвращает
     * другую функцию, имеющую на один аргумент меньше. См. https://ru.wikipedia.org/wiki/Каррирование
     */
    Calculator calc;

    calc.ProcessString(R"(plus = \ x y -> x + y)");
    calc.ProcessString(R"(IncreaseByTwo = plus 2)");
    assert(calc.ProcessString("IncreaseByTwo 10") == "12.000000");

    // Для интересующихся, что тут такое происходит: https://ru.wikipedia.org/wiki/Комбинаторная_логика
    calc.ProcessString(R"(K = \ x y -> x )");
    calc.ProcessString(R"(S = \ x y z -> x z ( y z ) )");
    calc.ProcessString(R"(I = S K K )");
    assert(calc.ProcessString("I 10") == "10.000000");
    calc.ProcessString("a = 55");
    assert(calc.ProcessString("I a") == "55.000000");
}

void TestParsingExceptions() {
    StatementParser parser;
    Context ctx;

    try {
        // незнакомые символы воспринимаются как имена только если они начинаются с буквы и состоят и букв и чисел
        parser.Tokenize("a = ?", ctx);
    } catch (const ParsingException& e) {
        assert(dynamic_cast<const Exception*>(&e));
        assert(dynamic_cast<const std::exception*>(&e));
        assert(e.what() == std::string("Cannot parse string: ?"));
    }

    try {
        // нельзя использовать операцию присваивания внутри выражений (expressions)
        // присваивание работает только внутри специальной инструкции (statement) присваивания
        parser.ParseString("5 = 10", ctx);
    } catch (const ParsingException& e) {
        assert(dynamic_cast<const Exception*>(&e));
        assert(dynamic_cast<const std::exception*>(&e));
        assert(e.what() == std::string("Cannot parse string: Cannot assign in expression"));
    }

    try {
        parser.ParseString("5 )", ctx);
    } catch (const ParenthesisMismatchException& e) {
        assert(dynamic_cast<const Exception*>(&e));
        assert(dynamic_cast<const std::exception*>(&e));
        assert(dynamic_cast<const ParsingException*>(&e));
        assert(e.what() == std::string("Cannot parse string: Parenthesis mismatch"));
    }

    try {
        parser.ParseString("( )", ctx);
    } catch (const EmptyParsingResultException& e) {
        assert(dynamic_cast<const Exception*>(&e));
        assert(dynamic_cast<const std::exception*>(&e));
        assert(dynamic_cast<const ParsingException*>(&e));
        assert(e.what() == std::string("Cannot parse string: Empty parsing result"));
    }

    try {
        parser.ParseString(R"(\ x 10 -> x)", ctx);
    } catch (const ParsingException& e) {
        assert(dynamic_cast<const Exception*>(&e));
        assert(dynamic_cast<const std::exception*>(&e));
        assert(dynamic_cast<const BadLambdaArgumentException*>(&e));
        assert(e.what() == std::string("Cannot parse string: Expected argument names for the lambda, got 10"));
    }

    try {
        Calculator calc;
        calc.ParseExpressionLine("a = 15");
    } catch (const ParsingException& e) {
        assert(dynamic_cast<const Exception*>(&e));
        assert(dynamic_cast<const std::exception*>(&e));
        assert(e.what() == std::string("Cannot parse string: Want expression but it is assignment statement"));
    }

    try {
        // будем считать, что лямбда без аргументов не имеет права на жизнь
        parser.ParseString(R"(\ -> 10)", ctx);
    } catch (const ParsingException& e) {
        assert(dynamic_cast<const Exception*>(&e));
        assert(dynamic_cast<const std::exception*>(&e));
        assert(dynamic_cast<const LambdaWithoutArgumentsException*>(&e));
        assert(e.what() == std::string("Cannot parse string: A lambda requires at least one argument"));
    }
}

void TestEvaluationExceptions() {
    Calculator calculator;

    try {
        calculator.ProcessString("a = b");
    } catch (const UnresolvedReferenceException& e) {
        assert(dynamic_cast<const Exception*>(&e));
        assert(dynamic_cast<const std::exception*>(&e));
        assert(e.what() == std::string("Unknown name b"));
    }

    try {
        calculator.ProcessString("a + 5");
    } catch (const UnresolvedReferenceException& e) {
        assert(dynamic_cast<const Exception*>(&e));
        assert(dynamic_cast<const std::exception*>(&e));
        assert(e.what() == std::string("Unknown name a"));
    }

    try {
        calculator.ProcessString(R"(( \ x -> x + 5 ) 10 20)");
    } catch (const ApplyException& e) {
        assert(dynamic_cast<const Exception*>(&e));
        assert(dynamic_cast<const std::exception*>(&e));
        assert(e.what() == std::string(R"(Cannot apply function 'λx.(+(x)(5.000000)) 10.000000' to argument 20.000000)"));
    }
}

int main() {
    TestSimpleMath();
    TestUnaryFunctions();
    TestLambda();
    TestAssignment();
    TestParsingExceptions();
    TestEvaluationExceptions();
    TestCurrying();
    return 0;
}