#include "iostream"
#include <vector>
#include <string>
#include "memory"
#include <cassert>
#include "unordered_map"
using namespace std;



vector<unique_ptr<string>> Duplicate(vector<shared_ptr<std::string>> vec){
    vector<unique_ptr<string>> ret;
    for (auto i: vec) {
        ret.push_back(unique_ptr<string>{make_unique<string>(*i)});
    }
    return ret;
}



vector<shared_ptr<std::string>> DeDuplicate(vector<unique_ptr<string>>& vec){
    vector<shared_ptr<string>> ret;
    unordered_map<string, shared_ptr<string>> m;
    for (int i = 0; i < (int)vec.size(); ++i) {
        if (!m.contains(*vec[i])){
            m[*vec[i]] = make_shared<string>(*vec[i]);
        }
    }
    for (int i = 0; i < (int)vec.size(); ++i) {
        ret.push_back(m[*vec[i]]);
    }
    return ret;

}