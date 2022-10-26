#include <cassert>
#include <algorithm>
#include "task.h"
#include "iostream"
using namespace std;

void Test(vector<int> initial) {
    vector<int> sortedByHeapSort = HeapSort(initial);
    sort(initial.begin(), initial.end());
    cout << sortedByHeapSort.size() << " . " << initial.size() << endl;
    assert(sortedByHeapSort.size() == initial.size());
    for (size_t i = 0; i < initial.size(); ++i) {
        assert(initial[i] == sortedByHeapSort[i]);
    }
}

int main() {
    Test({5, 2, 4, 3, 1, 4});
    Test({});

    return 0;
}
