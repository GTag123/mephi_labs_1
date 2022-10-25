#include "range.h"

#if defined(_LIBCPP_VECTOR) || defined(_LIBCPP_LIST) || defined(_LIBCPP_DEQUE) || defined(_LIBCPP_FORWARD_LIST)
#error "include vector, list or deque forbidden"
#endif

#include <cassert>
#include <vector>
#include <cstdint>

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
    std::cout << range.Size() << " " << vec.size() << std::endl;
    if (range.Size() != static_cast<int64_t>(vec.size())) {
        std::cout << "false" << std::endl;
        return false;
    }

    auto range_iter = range.begin();
    auto vec_iter = vec.begin();

    while (range.end() != range_iter && vec.end() != vec_iter) {
//        std::cout << "RI: " << *range_iter << std::endl;
//        std::cout << "VI: " << *vec_iter << std::endl;
        if (*vec_iter++ != *range_iter++) {
            std::cout << "false2" << std::endl;
            return false;
        }
    }
//    std::cout << "RI: " << *range_iter << std::endl;
//    std::cout << "VI: " << *vec_iter << std::endl;
//    std::cout << std::endl;
    if (!(range_iter == range.end() && vec_iter == vec.end())){
        std::cout << "errRI: " << *range_iter << "  r.end(): " << *(range.end()) << std::endl;
        std::cout << "errVI: " << *vec_iter << "  v.end(): " << *(vec.end()) << std::endl;
    }
    return range_iter == range.end() && vec_iter == vec.end();
}

void TestProperties() {
//    std::cout << sizeof(Range) << std::endl;
//    std::cout << sizeof(Range(123456790)) << std::endl;
//    std::cout << sizeof(Range(123456790)) << std::endl;
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
    std::cout << 83 << std::endl;
    assert(Range(100, -100, -111) == GenerateSequence(100, -100, -111));
    std::cout << 84 << std::endl;
    assert(Range(-100, 100, 200) == GenerateSequence(-100, 100, 200));
    assert(Range(-100, 100, 201) == GenerateSequence(-100, 100, 201));
    std::cout << 88 << std::endl;
    assert(Range(-5, -5, 3) == GenerateSequence(-5, -5, 3));
    assert(Range(-123, -12, 10) == GenerateSequence(-123, -12, 10));
    std::cout << 91 << std::endl;
    assert(Range(-12, -123, -10) == GenerateSequence(-12, -123, -10));

    assert(Range(-100, -1, 531) == GenerateSequence(-100, -1, 531));
    std::cout << 95 << std::endl;
}

void TestBig() {
    std::cout << "big" << std::endl;
    const int64_t kMaxN = 0xdead0beefLL;
    std::cout << "big11" << std::endl;
    const Range big_range(kMaxN);
    std::cout << "big111" << std::endl;
    assert(big_range.Size() == kMaxN);
    std::cout << "big12" << std::endl;
    auto big_range_iter = big_range.begin();
    std::cout << "big13" << std::endl;
    for (int i = 0; i < 1000000; ++i) {
        assert(*(big_range_iter++) == i);
    }
    std::cout << "big2" << std::endl;
    const Range large_range(kMaxN, -kMaxN, -1357);
    std::cout << "big3" << std::endl;
    auto large_range_iter = large_range.begin();
    std::cout << "big4" << std::endl;
    for (int i = 0; i < 1000000; ++i) {
        assert(*large_range_iter++ == kMaxN - i * 1357LL);
    }
    std::cout << "big5" << std::endl;
}

int main() {
    TestProperties();
    TestSimple();
    TestBig();
    return 0;
}
