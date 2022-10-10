#include <string>
#include <cassert>
#include "task.h"

void test(const char* lhs, const char* rhs) {
    char* result = concat(lhs, rhs);
    assert(std::string_view(result) == std::string(lhs) + std::string(rhs));
    delete[] result;
}

int main () {
    test("lol ", "I am here!");
    test("", "I am here!");
    test("Hi", "");
    test("", "");
    return 0;
}
