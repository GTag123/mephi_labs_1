#include "task.h"

using namespace std;

vector<int> HeapSort(const vector<int>& v) {
    priority_queue<int> m;
    for (int i: v){
        m.push(i);
    }
    vector<int> f;
    for (int i = 0; i < (int)m.size(); ++i) {
        f.push_back(m.top());
        m.pop();
    }
    return f;
}
