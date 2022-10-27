#include "task.h"
#include "iostream"
bool operator <(const SuperKey& lhs, const SuperKey& rhs) {
    return lhs.StrPart <= rhs.StrPart;
}

void PopulateMap(
    std::map<SuperKey, std::string>& map,
    const std::vector<std::pair<SuperKey, std::string>>& toAdd
) {
    for (std::pair<SuperKey, std::string> it: toAdd) {
        map[it.first] = it.second;
    }
}
