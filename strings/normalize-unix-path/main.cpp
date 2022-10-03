#include <cassert>
#include "task.h"

using namespace std;

int main() {
    assert(normalize("/../../foo/././bar/../bar/./baz//1.txt") == "/foo/bar/baz/1.txt");
    assert(normalize("../../foo/././bar/../bar/./baz//1.txt") == "../../foo/bar/baz/1.txt");
    assert(normalize("..") == "..");
    assert(normalize(".") == ".");
    assert(normalize("/.") == "/");
    assert(normalize("/..") == "/");
    assert(normalize("/../.") == "/");
    assert(normalize("../.") == "..");
    assert(normalize("file.txt") == "file.txt");
    return 0;
}
