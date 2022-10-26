#include "task.h"

using namespace std;

vector<int> HeapSort(const vector<int>& v) {
    priority_queue<int> m;
    for (int i: v){
        m.push(i);
    }
    vector<int> f(v.size());
    for (int i = (int)v.size() - 1; i >= 0; i--) {
        f[i] = m.top();
//        std::cout << f[i] << " fdsg";
        m.pop();
    }
    return f;
}
