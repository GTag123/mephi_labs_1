#include <iostream>
#include <cassert>
#include "task.h"

using namespace std;

int main() {
    assert(is_palindrome("abacaba"));
    assert(is_palindrome("abaccaba"));
    assert(!is_palindrome("abacdaba"));
    assert(is_palindrome(""));
    assert(is_palindrome("a"));
    assert(is_palindrome("aa"));
    assert(!is_palindrome("ab"));
    assert(is_palindrome("a a"));
    assert(is_palindrome("a A"));
    assert(!is_palindrome("a b"));
    assert(is_palindrome(" a "));
    assert(is_palindrome(" a  "));
    assert(is_palindrome("Do geese see God?"));
    assert(is_palindrome("                ?"));

    return 0;
}
