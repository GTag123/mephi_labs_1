#include <algorithm>
#include <cassert>
#include <chrono>
#include <ctime>
#include <iostream>
#include <random>
#include <string>
#include <unordered_set>
using namespace std;
bool IsValidKey(const std::string& key) {
    return key.size() < 17 &&
        std::all_of(key.begin(), key.end(), [](unsigned char c) {
            return std::isalnum(c) || c == '_';
        });
}

std::vector<std::string> GetKeys(size_t max_count) {
    std::vector<std::string> keys;

    std::string line;
    while (keys.size() < max_count) {
        if (!std::getline(std::cin, line)) {
            break;
        }
        cout << "line :" << line << endl;
        assert(IsValidKey(line));
        keys.push_back(std::move(line));
    }

    return keys;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> dist(50000, 100000);

    const size_t initial_size = dist(rng);
    std::unordered_set<std::string> strings(initial_size);

    const auto keys = GetKeys(15000);

    const auto start_time = std::clock();
    for (const auto& key : keys) {
        strings.insert(key);
    }
    const auto end_time = std::clock();

    const double elapsed_time = static_cast<double>(end_time - start_time) / CLOCKS_PER_SEC;
    std::cout << "Elapsed time: " << elapsed_time << " sec" << std::endl;

    if (elapsed_time > 1.5) {
        std::cout << "Victory!!!" << std::endl;
        return 0;
    }

    assert(false);
    return 1;
}
