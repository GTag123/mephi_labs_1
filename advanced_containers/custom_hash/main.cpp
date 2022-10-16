#include <cassert>
#include "task.h"

using namespace std;

int main() {
    unordered_map<SuperKey, std::string> hashMap;
    vector<pair<SuperKey, string>> toAdd = {
        make_pair(SuperKey{"Hello!", 28, 1.5}, "value 1"),
        make_pair(SuperKey{"It's me!", 15, 0.88}, "value 2"),
        make_pair(SuperKey{"mephi", 1511, 0}, "value 3"),
        make_pair(SuperKey{"Good Bye", 0, 13.37}, "value 4"),
    };

    PopulateHashMap(hashMap, toAdd);

    assert(hashMap.size() == toAdd.size());
    for (const auto& kv : toAdd) {
        assert(hashMap.find(kv.first) != hashMap.end());
        assert(hashMap[kv.first] == kv.second);
    }

    return 0;
}
