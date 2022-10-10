#include "array.h"

#include <cassert>
#include <sstream>
#include <array>

void CheckOutputOperator() {
    std::stringstream out;
    std::string ans;
    Array a = Array(5, out, 5);

    getline(out, ans);
    assert(ans == "Constructed with default. Result Array's capacity is 10 and size is 5, elements are: 5, 5, 5, 5, 5");

    out << a << "\n";
    getline(out, ans);
    assert(ans == "Result Array's capacity is 10 and size is 5, elements are: 5, 5, 5, 5, 5");
}

void CheckDefaultConstructor() {
    std::stringstream out;
    std::string ans;

    {
        Array array = Array(out);
        getline(out, ans);
        assert(ans == "Constructed. Result Array's capacity is 2 and size is 0");
    }
    getline(out, ans);
    assert(ans == "Destructed 0");
}

void CheckCopyConstructor() {
    std::stringstream out;
    std::string ans;

    {
        Array array = Array(2, out);
        getline(out, ans);
        assert(ans == "Constructed with default. Result Array's capacity is 4 and size is 2, elements are: 0, 0");

        Array other = Array(array);
        getline(out, ans);
        assert(ans == "Constructed from another Array. Result Array's capacity is 4 and size is 2, elements are: 0, 0");

        array << 10;
        out << array << '\n';
        getline(out, ans);
        assert(ans == "Result Array's capacity is 4 and size is 3, elements are: 0, 0, 10");

        other << 20;
        out << other << '\n';
        getline(out, ans);
        assert(ans == "Result Array's capacity is 4 and size is 3, elements are: 0, 0, 20");
    }
    getline(out, ans);
    assert(ans == "Destructed 3");
    getline(out, ans);
    assert(ans == "Destructed 3");
}

void CheckCopyConstructorWithResize() {
    std::stringstream out;
    std::string ans;

    {
        Array array = Array(out);
        getline(out, ans);
        assert(ans == "Constructed. Result Array's capacity is 2 and size is 0");
        array.Resize(10);
        for (int i = 9; i >= 0; --i) {
            array[i] = 9 - i;
        }

        Array other = Array(array);
        getline(out, ans);
        assert(ans == "Constructed from another Array. Result Array's capacity is 10 and size is 10, elements are: 9, 8, 7, 6, 5, 4, 3, 2, 1, 0");

        out << array << "\n";
        std::string first;
        getline(out, first);

        out << other << "\n";
        std::string second;
        getline(out, second);

        assert(first == second);
    }
    getline(out, ans);
    assert(ans == "Destructed 10");
    getline(out, ans);
    assert(ans == "Destructed 10");
}

void CheckClassSize() {
    std::stringstream out;
    Array a = Array(out);
    a.Resize(100);
    Array b = Array(out);
    b.Resize(500);
    Array c = Array(out);

    assert(sizeof(a) == 32);
    assert(sizeof(a) == sizeof(b));
    assert(sizeof(a) == sizeof(c));
}

void CheckConstructorWithDefaultValue() {
    std::stringstream out;
    std::string ans;

    Array a = Array(10, out, 5);

    getline(out, ans);
    assert(ans == "Constructed with default. Result Array's capacity is 20 and size is 10, elements are: 5, 5, 5, 5, 5, 5, 5, 5, 5, 5");

    Array b = a;
    getline(out, ans);
    assert(ans == "Constructed from another Array. Result Array's capacity is 20 and size is 10, elements are: 5, 5, 5, 5, 5, 5, 5, 5, 5, 5");
}

void CheckPushAndPopBack() {
    std::stringstream out;
    std::string ans;
    Array a = Array(2, out, 2);

    getline(out, ans);
    assert(ans == "Constructed with default. Result Array's capacity is 4 and size is 2, elements are: 2, 2");

    out << a << "\n";
    getline(out, ans);
    assert(ans == "Result Array's capacity is 4 and size is 2, elements are: 2, 2");

    a.PushBack(4);
    a.PushBack(123);
    a.PushBack(456);
    a << 9713 << 98;
    out << a << "\n";
    getline(out, ans);
    assert(ans == "Result Array's capacity is 8 and size is 7, elements are: 2, 2, 4, 123, 456, 9713, 98");

    a.Reserve(100);

    a.PopBack();
    a.PopBack();
    a.PopBack();
    a.PopBack();
    out << a << "\n";
    getline(out, ans);
    assert(ans == "Result Array's capacity is 100 and size is 3, elements are: 2, 2, 4");

    a.PopBack();
    a.PopBack();
    a.PopBack();
    out << a << "\n";
    getline(out, ans);
    assert(ans == "Result Array's capacity is 100 and size is 0");
}

void CheckCapacityWithResizeAndBoolOperator() {
    std::stringstream out;
    std::string ans;
    Array a = Array(5, out, 2);

    getline(out, ans);
    assert(ans == "Constructed with default. Result Array's capacity is 10 and size is 5, elements are: 2, 2, 2, 2, 2");

    out << a << "\n";
    getline(out, ans);
    assert(ans == "Result Array's capacity is 10 and size is 5, elements are: 2, 2, 2, 2, 2");

    assert(a.Capacity() == 10 && a.Size() == 5 && a);

    a.Resize(10);
    assert(a.Capacity() == 10 && a.Size() == 10 && a);
    a.Resize(1);
    a.Reserve(100);
    assert(a.Capacity() == 100 && a.Size() == 1 && a);
    a.Resize(0);
    a.Reserve(1);
    assert(a.Capacity() == 100 && a.Size() == 0 && !a);
}

void CheckConcat() {
    std::stringstream out;
    std::string ans;

    Array a = Array(out);
    getline(out, ans);
    assert(ans == "Constructed. Result Array's capacity is 2 and size is 0");

    Array b = Array(out);
    getline(out, ans);
    assert(ans == "Constructed. Result Array's capacity is 2 and size is 0");

    a << 123 << 345 << 456 << 1 << 5 << 81;
    b << 234 << 345 << 0 << 9;

    a << b;
    out << a << "\n";
    const std::array<std::string, 4> prefix = {"Result", "Array's", "capacity", "is"};
    for (const std::string& res : prefix) {
        out >> ans;
        assert(ans == res);
    }
    {
        int tmp;
        out >> tmp; //read capacity
    }
    getline(out, ans);
    assert(ans == " and size is 10, elements are: 123, 345, 456, 1, 5, 81, 234, 345, 0, 9"); // check suffix
}

void CheckComparisonOperator() {
    std::stringstream out;
    std::string ans;

    Array a = Array(out);
    Array b = Array(out);

    a << 123 << 345 << 456 << 1 << 5 << 81;
    b << 234 << 345 << 0 << 9;

    assert(a < b);
    assert(a <= b);
    assert(b > a);
    assert(b >= a);
    assert(a != b);

    a.Resize(2);
    a[0] = 234;
    b.Resize(2);
    assert(a == b);
    assert(a <= b);
}

int main() {
    CheckOutputOperator();
    CheckDefaultConstructor();
    CheckCopyConstructor();
    CheckCopyConstructorWithResize();
    CheckClassSize();
    CheckConstructorWithDefaultValue();
    CheckPushAndPopBack();
    CheckCapacityWithResizeAndBoolOperator();
    CheckConcat();
    CheckComparisonOperator();
}
