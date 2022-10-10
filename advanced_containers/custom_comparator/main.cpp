#include <cassert>
#include "task.h"

using namespace std;

int main() {
    std::map<SuperKey, string> map;
    PopulateMap(
        map,
        {
            make_pair(SuperKey{"aaa", 1, 6.0f}, "value1"),
            make_pair(SuperKey{"bbb", 2, 5.0f}, "value2"),
            make_pair(SuperKey{"aab", 3, 4.0f}, "value3"),
            make_pair(SuperKey{"aaa", 4, 3.0f}, "value4"),
            make_pair(SuperKey{"aaa", 5, 2.0f}, "value5"),
            make_pair(SuperKey{"bbb", 6, 1.0f}, "value6"),
        }
    );

    assert(map.size() == 6);
    for (std::map<SuperKey, string>::const_iterator it = next(map.begin()); it != map.end(); ++it) {
        assert(prev(it)->first.StrPart <= it->first.StrPart);
    }

    return 0;
}
