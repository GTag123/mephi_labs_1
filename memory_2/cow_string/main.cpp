#include "cow_string.h"

#define DEFINED(value) defined(_LIBCPP_##value)

#if DEFINED(DEQUE) || DEFINED(FORWARD_LIST) || DEFINED(LIST) || DEFINED(STRING) || DEFINED(VECTOR) || DEFINED(MEMORY)
#error "include deque, list, string, vector or memory forbidden"
#else
#include <deque>
#include <forward_list>
#include <list>
#include <memory>
#include <string>
#include <vector>
#endif

#include <cassert>
#include <chrono>
#include <random>

void TestProperties() {
    assert(sizeof(CowString) == sizeof(void*));
}

void TestSimple() {
    CowString string;

    assert(string.Size() == 0);
    assert(string.Capacity() == 2);

    string.Resize(3);
    assert(string.Size() == 3);

    string[1] = 'c';
    string[2] = 'a';

    assert(string.At(1) == 'c');
    assert(string.At(2) == 'a');

    assert(string.Back() == 'a');

    string.PushBack('z');
    assert(string.Size() == 4);
    assert(string.Capacity() > 4);
    assert(string.At(3) == 'z');
    assert(string.Back() == 'z');

    assert(string.Capacity() < 20);
    string.Reserve(20);
    assert(string.Size() == 4);

    const auto* addr3 = &string.At(3);
    for (char c = 'R'; string.Size() < 20; ++c) {
        string.PushBack(c);
        assert(&string.At(3) == addr3);
    }
}

void TestCowProperties() {
    CowString string;
    for (int i = 0; i < 5; ++i) {
        string.PushBack(static_cast<char>('a' + i));
    }

    const auto* addr2 = &string.At(2);

    {
        CowString copy_string(string);
        assert(copy_string.Size() == 5);
        assert(&copy_string.At(2) == addr2);
        assert(&copy_string.At(2) == addr2);
    }

    {
        CowString copy_string(string);
        assert(&copy_string.At(2) == addr2);

        copy_string[2] = '?';
        const auto* copy_addr2 = &copy_string.At(2);

        assert(copy_addr2 != addr2);
        assert(copy_string.At(2) == '?');

        copy_string[2] = '!';
        assert(&copy_string.At(2) == copy_addr2);
        assert(copy_string.At(2) == '!');
    }

    {
        CowString copy_string(string);
        assert(&copy_string.At(2) == addr2);

        copy_string.PushBack('?');
        assert(&copy_string.At(2) != addr2);
        assert(copy_string.At(2) == *addr2);
    }

    {
        CowString string2;
        string2.PushBack('0');
        string2.PushBack('1');

        CowString string3(string2);
        string2 = string;

        assert(&string2.At(2) == addr2);
        assert(string2.At(2) == string.At(2));

        assert(string3.Size() == 2);
        assert(string3.At(0) == '0');
        assert(string3.At(1) == '1');
    }
}

void TestStress() {
    CowString cow_string;
    std::string string;

    const auto start_ts = std::chrono::steady_clock::now();

    for (int i = 0; i < 1000000; ++i) {
        const char c = static_cast<char>(rand() % 26 + 'a');
        cow_string.PushBack(c);
        string += c;
    }

    for (int i = 0; i < 1000000; ++i) {
        auto copy = cow_string;
        assert(copy.At(i) == string[i]);
    }

    const std::chrono::duration<double> elapsed_time = std::chrono::steady_clock::now() - start_ts;
    assert(elapsed_time.count() < 0.5);
}

int main() {
    TestProperties();
    TestSimple();
    TestCowProperties();
    TestStress();
    return 0;
}
