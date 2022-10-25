#include "task.h"
bool is_palindrome(const std::string& s) {
    std::string st = s;
    st.erase(std::remove(st.begin(), st.end(), '?'), st.end());
    st.erase(std::remove(st.begin(), st.end(), ' '), st.end());
    for (int i = st.size()-1; i >= 0; --i) {
        if (tolower(st[st.size()-1-i]) != tolower(st[i])){
            return false;
        }
    }
    return true;
}
