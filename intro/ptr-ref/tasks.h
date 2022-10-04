#pragma once

namespace NPointers {
    void Increment(int* x);
    int Multiply(int a, int b, bool* isOver);
    long long ScalarProduct(const int* a, const int* b, int len);
    int SizeOfMaximumSquareOfCrosses(const char* a, int Gsize, int size);
    long long* MultiplyToLongLong(int a, int b);
}

namespace NReferences {

    void MultiplyInplace(int& a, int& b);
    int CompareArraysByAverage(const int* a, int size, int* b);
}
