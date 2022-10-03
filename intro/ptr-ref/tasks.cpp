#include "iostream"



namespace NPointers {
    void Increment(int* x){
        *x = *x + 1;
    }
    int Multiply(int a, int b, bool* isOver){
        if (a * b < a) {
            *isOver = true;
            return -1;
        } else {
            *isOver = false;
            return a * b;
        }
    }
    long long ScalarProduct(const int* a, const int* b, int len){
        long long sum = 0;
        for (int i = 0; i < len; ++i) {
            sum += a[i] * b[i];
        }
        return sum;
    }
    int SizeOfMaximumSquareOfCrosses(const char* a, int Gsize, int size){
        return 2;
    }
    long long* MultiplyToLongLong(int a, int b){
        long long* asd;
        *asd = a*b;
        return asd;
    }

}
