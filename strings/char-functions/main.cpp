#include <cassert>
#include "task.h"

void test_isalpha() {
    assert(isalpha('a'));
    assert(isalpha('A'));
    assert(isalpha('c'));
    assert(isalpha('D'));
    assert(!isalpha('5'));
    assert(!isalpha('\n'));
    assert(!isalpha(' '));
}

void test_tolower() {
    assert(tolower('a') == 'a');
    assert(tolower('A') == 'a');
    assert(tolower('d') == 'd');
    assert(tolower('5') == '5');
    assert(tolower(' ') == ' ');
    assert(tolower('\n') == '\n');
}

int main() {
    test_isalpha();
    test_tolower();
    return 0;
}
