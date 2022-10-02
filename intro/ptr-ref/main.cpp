#include "tasks.h"

#include <cassert>
#include <iostream>

void CheckPointers() {
    {
        int x = 10;
        NPointers::Increment(&x);
        assert(x == 11);
    }

    {
        int a = 2097151;
        int b = 1024;
        bool isOverflow;
        int result = NPointers::Multiply(a, b, &isOverflow);
        assert(result == 2147482624 && !isOverflow);

        a = 1024;
        b = 2097152;
        result = NPointers::Multiply(a, b, &isOverflow);
        assert(isOverflow);
    }

    {
        const int a[] = {1,    2,   3,  4, 5,  6, 7,   8,    9, 10};
        const int b[] = {12, 345, 123, 91, 0, 19, 1, 342, 1231, 10};
        assert(NPointers::ScalarProduct(a, b, sizeof(a) / sizeof(a[0])) == 15471);
    }

    {
        const char a[3][5] = {{'.', '.', '+', '+', '+'},
                              {'+', '.', '.', '+', '+'},
                              {'.', '+', '+', '+', '+'}};
        assert(NPointers::SizeOfMaximumSquareOfCrosses((const char*) a, sizeof(a) / sizeof(a[0]), sizeof(a[0]) / sizeof(a[0][0])) == 2);
    }

    {
        const int a = 2097152;
        const int b = 2014;
        long long* c = NPointers::MultiplyToLongLong(a, b);
        assert(*c == 4223664128ll);
        delete c;
    }
}

void CheckReferences() {
    {
        int a = 1231;
        int b = 234;
        NReferences::MultiplyInplace(a, b);
        assert(a == 288054);
    }

    {
        const int a[] = {1, 2, 3, 4, 5, 6};
        int b[] = {1, 2, 3, 4, 5};
        assert(NReferences::CompareArraysByAverage(a, sizeof(a) / sizeof(a[0]), b) == 1);
    }

    {
        const int a[] = {1, 2, 3, 4, 5, 1};
        int b[] = {1, 2, 3, 4, 5};
        assert(NReferences::CompareArraysByAverage(a, sizeof(a) / sizeof(a[0]), b) == -1);
    }

    {
        const int a[] = {1, 2, 3, 4, 5, 3};
        int b[] = {1, 2, 3, 4, 5};
        assert(NReferences::CompareArraysByAverage(a, sizeof(a) / sizeof(a[0]), b) == 0);
    }
}

int main() {
    CheckPointers();
    CheckReferences();
    return 0;
}
