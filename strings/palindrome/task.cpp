#include "task.h"
bool is_palindrome(const std::string& s) {
    std::string st;
    for (int i = 0; i < (int)s.size(); ++i) {
        if (s[i] != ' ' && s[i] != '?') st += s[i];
    }
    for (int i = st.size()-1; i >= 0; --i) {
        if (tolower(st[st.size()-1-i]) != tolower(st[i])){
            return false;
        }
    }
    return true;
}
