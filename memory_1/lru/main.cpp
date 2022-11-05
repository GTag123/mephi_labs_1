#include "lru.h"

#include <cassert>
#include <chrono>
#include <random>
#include <string>
#include <vector>

struct TestKey {
    int first;
    int second;
};

bool operator==(const TestKey& lhs, const TestKey& rhs) {
    return lhs.first == rhs.first && lhs.second == rhs.second;
}

struct TestValue {
    std::string value;
};

bool operator==(const TestValue& lhs, const TestValue& rhs) {
    return lhs.value == rhs.value;
}

template <>
struct std::hash<TestKey> {
    size_t operator()(const TestKey& x) const noexcept {
        return static_cast<size_t>(x.first ^ (x.second << 10));
    }
};

void TestProperties() {
    LruCache<TestKey, TestValue> cache(1);

    TestKey key{1, 2};
    cache.Put(key, {"value"});

    TestValue result;
    assert(cache.Get(key, &result));
    assert(result == TestValue{"value"});

    cache.Put({-1, -2}, {"vz"});
    assert(!cache.Get(key, &result));
    assert(cache.Get({-1, -2}, &result));
    assert(result == TestValue{"vz"});
}

void TestSimple() {
    LruCache<std::string, std::string> cache(10);

    cache.Put("a", "1");
    cache.Put("b", "2");
    cache.Put("c", "3");
    cache.Put("d", "4");

    std::string result;
    for (int k = 0; k < 4; ++k) {
        assert(cache.Get(std::string(1, 'a' + k), &result));
        assert(result == std::string(1, '1' + k));
    }

    assert(!cache.Get("z", &result));

    cache.Put("c", "4");
    assert(cache.Get("c", &result));
    assert(result == "4");
}

void TestEviction() {
    LruCache<int, std::string> cache(2);

    cache.Put(1, "a");
    cache.Put(2, "b");
    cache.Put(3, "c");

    std::string result;
    assert(!cache.Get(1, &result));
    assert(cache.Get(3, &result));
    assert(cache.Get(2, &result));

    cache.Put(4, "d");
    assert(!cache.Get(3, &result));
    assert(cache.Get(2, &result));
    assert(result == "b");
    assert(cache.Get(4, &result));
    assert(result == "d");

    cache.Put(4, "e");
    assert(cache.Get(2, &result));
    assert(result == "b");
    assert(cache.Get(4, &result));
    assert(result == "e");

    cache.Put(2, "f");
    assert(cache.Get(2, &result));
    assert(result == "f");
    assert(cache.Get(4, &result));
    assert(result == "e");

    cache.Put(4, "g");
    assert(cache.Get(4, &result));
    assert(result == "g");
    assert(cache.Get(2, &result));
    assert(result == "f");

    cache.Put(5, "h");
    assert(!cache.Get(4, &result));
    assert(cache.Get(5, &result));
    assert(cache.Get(2, &result));

    cache.Put(6, "i");
    assert(!cache.Get(5, &result));
    assert(cache.Get(2, &result));
    assert(result == "f");
    assert(cache.Get(6, &result));
    assert(result == "i");
}

void TestStress() {
    std::mt19937 rnd(0xDEADBEEF);

    const auto start_ts = std::chrono::steady_clock::now();

    LruCache<std::string, int> cache(100);

    for (size_t i = 0; i < 200000; ++i) {
        if (rnd() & 1) {
            int result = 0;
            if (cache.Get(std::to_string(rnd() % 543), &result)) {
                assert(result == -1234);
            }
        } else {
            cache.Put(std::to_string(rnd() % 543), -1234);
        }
    }

    const std::chrono::duration<double> elapsed_time = std::chrono::steady_clock::now() - start_ts;
    assert(elapsed_time.count() < 0.5);
}

int main() {
    TestProperties();
    TestSimple();
    TestEviction();
    TestStress();
    return 0;
}
