#include <vector>
#include <memory>
#include <unordered_map>
#include <cassert>
#include <memory>
#include <string>
#include <vector>

std::vector<std::unique_ptr<std::string>> Duplicate(const std::vector<std::shared_ptr<std::string>>& in) {
    std::vector<std::unique_ptr<std::string>> out(in.size());
    for (size_t i = 0; i < out.size(); i++) {
        out[i] = std::make_unique<std::string>(*in[i]);
    }
    return out;
}

std::vector<std::shared_ptr<std::string>> DeDuplicate(const std::vector<std::unique_ptr<std::string>>& in) {
    std::unordered_map<std::string , int> help_map; // вспомогательный контейнер для строк, которые уже были
    std::vector<std::shared_ptr<std::string>> out(in.size());
    for (size_t i = 0; i < out.size(); ++i) {
        if (help_map.find(*in[i]) == help_map.end()) { // элемента не было до данной итерации цикла
            out[i] = std::make_shared<std::string>(*in[i]);
            help_map[*in[i]] = i;
        } else { // элемент уже встречался ранее
            out[i] = out[help_map.find(*in[i])->second];
        }
    }
    return out;
}