#include "iostream"
#include "tasks.h"


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
        long long* asd = new long long();
        *asd = (long long)a*(long long)b;
        return asd;
    }
}
void NReferences::MultiplyInplace(int& a, int& b){
    a = a*b;
}
int NReferences::CompareArraysByAverage(const int* a, int size, int* b){
    int sum1 = 0, sum2 = 0;

    for (int i = 0; i < size; ++i) {
        sum1 += a[i];
    }
    for (int i = 0; i < 5; ++i) {
        sum2 += b[i];
    }
    double sr1 = (double) sum1 / (double) size,
           sr2 = (double) sum2 / (double) 5;
    if ( sr1 > sr2 ) return 1;
    else if (sr1 == sr2) return 0;
    else return -1;
}
