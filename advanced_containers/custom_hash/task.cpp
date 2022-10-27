#include "task.h"

using namespace std;

namespace std {
    size_t  std::hash<SuperKey>::operator()(const SuperKey& k) const
        {
            using std::size_t;
            using std::hash;
            using std::string;

            hash<string> ab = hash<string>();
            return ab(k.StrPart);
        }
    };
void PopulateHashMap(
    unordered_map<SuperKey, string>& hashMap,
    const vector<pair<SuperKey, string>>& toAdd) {
    for (int i = 0; i < (int)toAdd.size(); ++i) {
        hashMap[toAdd[i].first] = toAdd[i].second;
    }
}
