#include "range.h"

#if defined(_LIBCPP_VECTOR) || defined(_LIBCPP_LIST) || defined(_LIBCPP_DEQUE) || defined(_LIBCPP_FORWARD_LIST)
#error "include vector, list or deque forbidden"
#endif

#include <cassert>
#include <cmath>
#include <vector>

namespace {

    std::vector<int64_t> GenerateSequence(int64_t start, int64_t end, int64_t step) {
        std::vector<int64_t> sequence;
        if (start != end) {
            sequence.push_back(start);
        }
        for (int64_t current = start; std::abs(end - current) > std::abs(step); current += step) {
            sequence.push_back(current + step);
        }
        return sequence;
    }

    std::vector<int64_t> GenerateSequence(int64_t size) {
        return GenerateSequence(0, size, 1);
    }

}

bool operator==(const Range& range, const std::vector<int64_t>& vec) {
    if (range.Size() != static_cast<int64_t>(vec.size())) {
        return false;
    }

    auto range_iter = range.begin();
    auto vec_iter = vec.begin();
    while (range.end() != range_iter && vec.end() != vec_iter) {
        if (*vec_iter++ != *range_iter++) {
            return false;
        }
    }

    return range_iter == range.end() && vec_iter == vec.end();
}

void TestProperties() {
    assert(sizeof(Range) < 25);
    assert(sizeof(Range(123456790).begin()) < 25);
    assert(sizeof(Range(123456790).end()) < 25);

    int64_t iter = -10;
    for (auto x : Range(-10, 10)) {
        assert(x == iter++);
    }
    assert(iter == 10);
}

void TestSimple() {
    assert(Range(100) == GenerateSequence(100));
    assert(Range(0) == GenerateSequence(0));
    assert(Range(1) == GenerateSequence(1));

    assert(Range(10, 99, 34) == GenerateSequence(10, 99, 34));
    assert(Range(10, 99, 3400) == GenerateSequence(10, 99, 3400));
    assert(Range(88, 11, -3) == GenerateSequence(88, 11, -3));

    assert(Range(-100, 99, 7) == GenerateSequence(-100, 99, 7));
    assert(Range(99, -100, -5) == GenerateSequence(99, -100, -5));
    assert(Range(100, -100, -111) == GenerateSequence(100, -100, -111));
    assert(Range(-100, 100, 200) == GenerateSequence(-100, 100, 200));
    assert(Range(-100, 100, 201) == GenerateSequence(-100, 100, 201));

    assert(Range(-5, -5, 3) == GenerateSequence(-5, -5, 3));
    assert(Range(-123, -12, 10) == GenerateSequence(-123, -12, 10));
    assert(Range(-12, -123, -10) == GenerateSequence(-12, -123, -10));
    assert(Range(-100, -1, 531) == GenerateSequence(-100, -1, 531));
}

void TestBig() {
    const int64_t kMaxN = 0xdead0beefLL;

    const Range big_range(kMaxN);
    assert(big_range.Size() == kMaxN);

    auto big_range_iter = big_range.begin();
    for (int i = 0; i < 1000000; ++i) {
        assert(*big_range_iter++ == i);
    }

    const Range large_range(kMaxN, -kMaxN, -1357);
    auto large_range_iter = large_range.begin();
    for (int i = 0; i < 1000000; ++i) {
        assert(*large_range_iter++ == kMaxN - i * 1357LL);
    }
}

int main() {
    TestProperties();
    TestSimple();
    TestBig();
    return 0;
}
