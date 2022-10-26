#include "task.h"
#include "iostream"
char* concat(const char* lhs, const char* rhs) {
    int i = 0, j = 0;
    while (*(lhs+i)!='\0') i++;
    while (*(rhs+j)!='\0') j++;
    char* n = new char[i+j+1];
    for (int k = 0; k < i; ++k) {
        n[k] = *(lhs+k);
    }
    for (int k = 0; k < j; ++k) {
        n[i+k] = *(rhs+k);
    }
    n[i+j] = '\0';
//    std::cout << "Otl:\n";
//    for (int k = 0; n[k] != '\0'; ++k) {
//        std::cout << n[k];
//    }
//    std::cout << "<" << std::endl << "end" << std::endl;
    return n;
}
