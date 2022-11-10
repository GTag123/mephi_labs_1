#include "task.h"

#include <cassert>
#include <memory>
#include <string>

void TestDuplication() {
    const auto first = std::make_shared<std::string>("first");
    const auto second = std::make_shared<std::string>("second");
    const auto third = std::make_shared<std::string>("third");

    const std::vector<std::shared_ptr<std::string>> dedup {first, second, third, third, first, second};
    const auto dup = Duplicate(dedup);

    assert(*dup[0] == "first");
    assert(*dup[1] == "second");
    assert(*dup[2] == "third");
    assert(*dup[3] == "third");
    assert(*dup[4] == "first");
    assert(*dup[5] == "second");

    assert(dup[0] != dup[4]);
    assert(dup[1] != dup[5]);
    assert(dup[2] != dup[3]);

    assert(dup[0].get() != first.get());
    assert(dup[1].get() != second.get());
    assert(dup[2].get() != third.get());
}

void TestDeDuplication() {
    std::vector<std::unique_ptr<std::string>> dup;
    dup.push_back(std::make_unique<std::string>("1"));
    dup.push_back(std::make_unique<std::string>("2"));
    dup.push_back(std::make_unique<std::string>("3"));
    dup.push_back(std::make_unique<std::string>("1"));
    dup.push_back(std::make_unique<std::string>("3"));
    dup.push_back(std::make_unique<std::string>("2"));

    const auto dedup = DeDuplicate(dup);

    assert(*dedup[0] == "1");
    assert(*dedup[1] == "2");
    assert(*dedup[2] == "3");
    assert(*dedup[3] == "1");
    assert(*dedup[4] == "3");
    assert(*dedup[5] == "2");

    assert(dedup[0] == dedup[3]);
    assert(dedup[1] == dedup[5]);
    assert(dedup[2] == dedup[4]);
}

void TestDuplication2() {
    const auto value = std::make_shared<std::string>("value");
    const std::vector<std::shared_ptr<std::string>> dedup {value ,value, value, value, value};

    const auto dup = Duplicate(dedup);

    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < i; ++j) {
            assert(dup[i] != dup[j]);
        }
    }

    for (int i = 0; i < 5; ++i) {
        assert(*dup[i] == "value");
    }
}

#include <iostream>
void TestDeDuplication2() {
    std::vector<std::unique_ptr<std::string>> dup;
    for (int i = 0; i < 5; ++i) {
        dup.push_back(std::make_unique<std::string>("value"));
    }

    const auto dedup = DeDuplicate(dup);

    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < i; ++j) {
            assert(dedup[i] == dedup[j]);
        }
    }

    for (int i = 0; i < 5; ++i) {
        assert(*dedup[i] == "value");
    }
}

int main() {
    TestDuplication();
    TestDeDuplication();
    TestDuplication2();
    TestDeDuplication2();
    return 0;
}
