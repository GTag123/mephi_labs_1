#include <string>
#include "iostream"
#include <iterator>
#include <vector>
using namespace std;
void printv(vector<string> v){
    for (int i = 0; i < (int)v.size(); ++i) {
        cout << v[i];
        if (v[i] == "") {
            cout << " empty";
        }
        cout << endl;
    }
}
vector<string> split(const string& str){
    vector<string> dirs;
    string pull = "";
    int i = 0;
    while (i < (int)str.size()){
        for (; str[i] != '/' && i < (int)str.size(); i++) {
            pull += str[i];
        }
        dirs.push_back(pull);
        i++;
        pull = "";
    }
    return dirs;
}


string normalize(string_view path) {
    string st;
    vector<string> dirs = split(string(path));
    vector<string> depthdirs;
    int depth;
    int i = 0;
    if (dirs[0] == ""){
        depth = 0;
        i++;
        st += '/';
    }
    else depth = 99999999;
    printv(dirs);
    for (; i < (int)dirs.size(); ++i) {
        if (dirs[i] == ".."){
            if (depth != 0 || (int)depthdirs.size() > 0) {
                if ((int)depthdirs.size() > 0 && depthdirs[(int)depthdirs.size() - 1] != ".."){
                    depthdirs.pop_back();
                } else {
                    depthdirs.push_back(dirs[i]);
                }
            }
        } else if (dirs[i] != "." && dirs[i] != ""){
            depthdirs.push_back(dirs[i]);
        }
    }
    if ((int)path.size() == 1 && path[0] == '.'){
        depthdirs.push_back(".");
    }
    for (int j = 0; j < (int)depthdirs.size(); ++j) {
        st += depthdirs[j];
        if (j < (int)depthdirs.size() - 1) st += "/";
    }
    cout << st << endl;
    return st;
}
