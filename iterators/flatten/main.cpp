#include "flatten.h"

#if defined(_LIBCPP_LIST) || defined(_LIBCPP_DEQUE) || defined(_LIBCPP_FORWARD_LIST)
#error "include list or deque forbidden"
#endif

#include <algorithm>
#include <cassert>
#include <chrono>
#include <memory>
#include <random>
#include <string>
#include <vector>

namespace {

    struct TestData {
        int first;
        char second;
        int64_t third;
        std::string fourth;
    };

    std::mt19937 g_random_engine(11);

    template <typename T, typename Generator>
    std::vector<std::vector<T>> GenerateSample(int total_count, int empty_count, Generator generator) {
        assert(total_count >= empty_count);

        std::vector<std::vector<T>> sample(total_count);

        std::uniform_int_distribution<int> size_dist(1, 5);
        for (int i = empty_count; i < total_count; ++i) {
            const int size = size_dist(g_random_engine);
            for (int j = 0; j < size; ++j) {
                sample[i].push_back(generator());
            }
        }

        std::shuffle(sample.begin(), sample.end(), g_random_engine);

        return sample;
    }

    template <typename T>
    std::vector<T> Flatten(const std::vector<std::vector<T>>& vector) {
        std::vector<T> result;

        for (const auto& sub_vector : vector) {
            for (const auto& el : sub_vector) {
                result.push_back(el);
            }
        }

        return result;
    }

}

void TestProperties() {
    {
        assert(sizeof(FlattenedVector<double>) <= 40);
        assert(sizeof(FlattenedVector<TestData>) <= 40);
        assert(sizeof(std::declval<FlattenedVector<TestData>>().begin()) <= 32);
        assert(sizeof(std::declval<FlattenedVector<TestData>>().end()) <= 32);
    }

    {
        auto vector = GenerateSample<int64_t>(10, 3, g_random_engine);
        const auto flattened = Flatten(vector);
        int i = 0;
        for (auto x : FlattenedVector<int64_t>(vector)) {
            assert(x == flattened[i++]);
        }
    }

    {
        auto vector = GenerateSample<int>(100, 10, g_random_engine);
        const auto expected = Flatten(vector);

        FlattenedVector<int> flattened_vector(vector);

        assert(std::distance(flattened_vector.begin(), flattened_vector.end()) == static_cast<ssize_t>(expected.size()));

        auto iter = flattened_vector.begin() + 33;
        assert(((void)[](auto x){ ++x; }(iter), *iter) == *iter);

        const auto it_a = flattened_vector.begin() + 11;
        const auto it_b = flattened_vector.begin() + 42;
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
    std::vector<std::vector<int>> vector = {
        {},
        {0},
        {1, 2, 3, 4, 5, 6, 7},
        {},
        {},
        {8, 9},
        {10},
        {},
        {11},
        {12, 13, 14, 15, 16},
        {17, 18},
        {},
        {19},
        {},
        {20, 21, 22},
        {},
        {},
    };

    FlattenedVector fv(vector);

    auto it = fv.begin();
    assert(it[10] == 10);
    assert(it[0] == 0);
    assert(it[15] == 15);

    assert(*it == 0);
    it += 10;
    assert(*it == 10);
    it -= -3;
    assert(*it == 13);
    it -= 2;
    assert(*it == 11);
    --it;
    assert(*it == 10);
    it += 6;
    assert(*it == 16);
    it += 3;
    assert(*it == 19);
    it += -2;
    assert(*it == 17);
    it -= 9;
    assert(*it == 8);
    it += -8;
    assert(*it == 0);
    it -= -22;
    assert(*it == 22);
}

void TestEmpty() {
    for (auto vector : {std::vector<std::vector<TestData>>{}, std::vector<std::vector<TestData>>{{}, {}}}) {
        FlattenedVector<TestData> fv(vector);
        assert(std::distance(fv.begin(), fv.end()) == 0);
        assert(fv.begin() == fv.end());
        for (auto x : fv) {
            assert(false);
        }
    }
}

void TestModify() {
    {
        auto vector = GenerateSample<char>(100, 10, []() { return 'a' + g_random_engine() % 26; });
        FlattenedVector<char> flattened_vector(vector);

        const auto it = flattened_vector.begin() + 42;
        const char old_value = *it;
        const char new_value = old_value + 1;
        *it = new_value;
        assert(*it == new_value);

        const char value = it[1] == old_value ? old_value : new_value;
        it[1] = value;
        assert(*(it + 1) == value);
    }

    {
        auto vector = GenerateSample<int>(1000, 100, g_random_engine);
        auto expected = Flatten(vector);
        std::sort(expected.begin(), expected.end());

        FlattenedVector<int> flattened_vector(vector);
        std::sort(flattened_vector.begin(), flattened_vector.end());

        assert(Flatten(vector) == expected);
    }
}

void TestStress() {
    auto test = [](std::vector<std::vector<uint64_t>> vector) {
        const auto flattened = Flatten(vector);
        const auto size = flattened.size();

        const auto start_ts = std::chrono::steady_clock::now();

        FlattenedVector<uint64_t> flattened_vector(vector);
        auto flattening_iter = flattened_vector.begin();
        for (int i = 0, pos = 0; i < 100000; ++i) {
            assert(std::distance(flattened_vector.begin(), flattening_iter) == pos);
            int new_pos = 0;
            if (i & 1) {
                std::uniform_int_distribution<int> step_dist(0, size / 100);
                new_pos = step_dist(g_random_engine);
            } else {
                std::uniform_int_distribution<int> step_dist(9 * size / 100, size - 1);
                new_pos = step_dist(g_random_engine);
            }
            flattening_iter += new_pos - pos;
            pos = new_pos;
            assert(flattened[pos] == *flattening_iter);
        }

        const std::chrono::duration<double> elapsed_time = std::chrono::steady_clock::now() - start_ts;
        assert(elapsed_time.count() < 0.5);
    };

    {
        auto vector = GenerateSample<uint64_t>(200000, 184321, g_random_engine);
        test(vector);
    }

    {
        auto vector = GenerateSample<uint64_t>(200000, 1234, g_random_engine);
        test(vector);
    }

    {
        std::vector<std::vector<int>> vector{{1}};
        for (int i = 0; i < 100000; ++i) {
            vector.emplace_back(std::vector<int>{});
        }
        vector.emplace_back(std::vector<int>{2, 1});

        const auto start_ts = std::chrono::steady_clock::now();

        FlattenedVector<int> flattened_vector(vector);
        auto flattening_iter = flattened_vector.begin();
        for (int i = 0; i < 100000; ++i) {
            if (i & 1) {
                --flattening_iter;
                assert(*flattening_iter == 1);
            } else {
                ++flattening_iter;
                assert(*flattening_iter == 2);
            }
        }
        ++flattening_iter;
        ++flattening_iter;
        assert(*flattening_iter == 1);
        --flattening_iter;
        assert(*flattening_iter == 2);
        --flattening_iter;
        assert(*flattening_iter == 1);

        const std::chrono::duration<double> elapsed_time = std::chrono::steady_clock::now() - start_ts;
        assert(elapsed_time.count() < 0.5);
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
