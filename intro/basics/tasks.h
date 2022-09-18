#pragma once

#include <cstdint>

namespace NOuter {
    namespace NInner {
        int DoSomething(int lhs, int rhs);
    }
    int DoSomething(int lhs, int rhs);
}

namespace NOverload {
    int ProcessTwoArgs(int lhs, int rhs);
    char ProcessTwoArgs(char lhs, char rhs);
    int ProcessTwoArgs(int lhs, char rhs);
}

namespace NOverflow {
    unsigned int WithOverflow(int lhs, int rhs);
    uint64_t WithOverflow(int64_t lhs, int64_t rhs);
}

namespace NLoop {
    int SumInRange(const int lhs, const int rhs);
    int CountFixedBitsInRange(const int from, const int to, const int bitsCnt);
}

namespace NMath {
    double ComputeMathFormula(const double arg);
    bool IsPositive(int arg);
}

namespace NRecursion {
    int CalculateFibonacci(const int arg);
}
