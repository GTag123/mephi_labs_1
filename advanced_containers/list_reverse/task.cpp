#include "task.h"

void ReverseList(std::list<int>& l) {
    auto b = l.rbegin();
    std::list<int> l2;
    while (b != l.rend()){
        l2.push_back(*b++);
    }
    l = l2;
}
