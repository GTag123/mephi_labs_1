#include "array.h"

#if defined(_LIBCPP_VECTOR) || defined(_LIBCPP_LIST) || defined(_LIBCPP_DEQUE) || defined(_LIBCPP_FORWARD_LIST)
#error "include vector, list or deque forbidden"
#endif

#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>
#include <string>
#include <utility>
#include <vector>

namespace {

    struct TestData {
        int first;
        char second;
        int64_t third;
        std::string fourth;

        bool operator==(const TestData& rhs) const {
            return first == rhs.first && second == rhs.second && third == rhs.third && fourth == rhs.fourth;
        }
    };

    std::ostream& operator<<(std::ostream& out, const TestData& data) {
        out << '{' << data.first << ", " << data.second << ", " << data.third << ", " << data.fourth << '}';
        return out;
    }

    std::mt19937 g_random_engine(11);

    template <typename T, typename Generator>
    std::pair<Array<T>, std::vector<T>> GenerateSample(int size, Generator generator) {
        static std::ostream devnull(nullptr);
        std::vector<T> vector(size);
        Array<T> array(size, devnull);

        for (int i = 0; i < size; ++i) {
            vector[i] = generator();
            array[i] = vector[i];
        }

        return {array, vector};
    }

}

void TestProperties() {
    assert(sizeof(std::declval<Array<TestData>>().begin()) <= sizeof(void*));
    assert(sizeof(std::declval<Array<TestData>>().end()) <= sizeof(void*));

    {
        const auto [array, vector] = GenerateSample<int>(10, g_random_engine);

        int i = 0;
        for (auto x : array) {
            assert(x == vector[i++]);
        }
    }

    {
        const auto [array, vector] = GenerateSample<int64_t>(100, g_random_engine);

        assert(std::distance(array.begin(), array.end()) == static_cast<ssize_t>(array.Size()));

        auto iter = array.begin() + 33;
        assert(((void)[](auto x){ ++x; }(iter), *iter) == *iter);

        const auto it_a = array.begin() + 11;
        const auto it_b = array.begin() + 42;
        const auto diff = it_b - it_a;

        assert(diff == 42 - 11);

        assert(it_a <= it_b);
        assert(it_b != it_a);
        assert(it_b > it_a);

        assert((it_a + diff) == (diff + it_a));
        assert((it_a + 0) == it_a);
        assert(it_a[diff] == *it_b);

        [diff](auto a, auto b) {
            assert((a += diff) == b);
        }(it_a, it_b);
        [diff](auto a, auto b) {
            assert((b -= diff) == a);
        }(it_a, it_b);
        [diff](auto a) {
            assert(std::addressof(a += diff) == std::addressof(a));
        }(it_a);
        [diff](auto b) {
            assert(std::addressof(b -= diff) == std::addressof(b));
        }(it_b);
        [it_a, diff](auto a) {
            assert((it_a + diff) == (a += diff));
        }(it_a);
        [it_b, diff](auto b) {
            assert((it_b - diff) == (b -= diff));
        }(it_b);
        [diff](auto a, auto b) {
            assert((a + (diff - 1)) == (--b));
        }(it_a, it_b);
    }
}

void TestSimple() {
    const auto [array, vector] = GenerateSample<int>(25, g_random_engine);

    auto array_it = array.begin();
    assert(array_it[10] == vector[10]);
    assert(array_it[0] == vector[0]);
    assert(array_it[15] == vector[15]);

    auto vector_it = vector.begin();

    assert(*array_it == *vector_it);
    array_it += 10;
    vector_it += 10;
    assert(*array_it == *vector_it);
    array_it -= -3;
    vector_it -= -3;
    assert(*array_it == *vector_it);
    array_it -= 2;
    vector_it -= 2;
    assert(*array_it == *vector_it);
    --array_it;
    --vector_it;
    assert(*array_it == *vector_it);
    array_it += 6;
    vector_it += 6;
    assert(*array_it == *vector_it);
    array_it += 3;
    vector_it += 3;
    assert(*array_it == *vector_it);
    array_it += -2;
    vector_it += -2;
    assert(*array_it == *vector_it);
    array_it -= 9;
    vector_it -= 9;
    assert(*array_it == *vector_it);
    array_it += -8;
    vector_it += -8;
    assert(*array_it == *vector_it);
    array_it -= -22;
    vector_it -= -22;
    assert(*array_it == *vector_it);
}

void TestEmpty() {
    const auto [array, vector] = GenerateSample<TestData>(0, [](){ return TestData(); });
    assert(std::distance(array.begin(), array.end()) == 0);
    assert(array.begin() == array.end());
    for (auto x : array) {
        assert(false);
    }
}

void TestModify() {
    {
        auto [array, vector] = GenerateSample<char>(100, []() { return 'a' + g_random_engine() % 26; });

        const auto it = array.begin() + 42;
        const char old_value = *it;
        const char new_value = old_value + 1;
        *it = new_value;
        assert(*it == new_value);

        const char value = it[1] == old_value ? old_value : new_value;
        it[1] = value;
        assert(*(it + 1) == value);
    }

    {
        auto [array, vector] = GenerateSample<TestData>(1, []() { return TestData(); });

        auto modify = []<typename T>(T* container) {
            container->begin()->first = 1;
            container->begin()->second = 'b';
            container->begin()->third = 333333333333LL;
            container->begin()->fourth = "abcd";
        };

        modify(&array);
        modify(&vector);

        assert(array[0] == vector[0]);
    }

    {
        auto [array, vector] = GenerateSample<int>(1000, g_random_engine);
        std::sort(vector.begin(), vector.end());
        std::sort(array.begin(), array.end());

        assert(std::vector<int>(array.begin(), array.end()) == vector);
    }
}

void TestStress() {
    auto [array, vector] = GenerateSample<uint64_t>(200000, g_random_engine);

    const auto size = vector.size();

    auto iter = array.begin();
    for (int i = 0, pos = 0; i < 100000; ++i) {
        assert(std::distance(array.begin(), iter) == pos);
        int step = 0;
        if (i & 1) {
            std::uniform_int_distribution<int> step_dist(0, pos);
            step = -step_dist(g_random_engine);
        } else {
            std::uniform_int_distribution<int> step_dist(0, size - pos - 1);
            step = step_dist(g_random_engine);
        }
        pos += step;
        iter += step;
        assert(vector[pos] == *iter);
    }
}

int main() {
    TestProperties();
    TestSimple();
    TestEmpty();
    TestModify();
    TestStress();
    return 0;
}
