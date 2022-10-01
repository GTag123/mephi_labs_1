#include "tasks.h"
#include <cmath>
namespace NOuter {
    namespace NInner {
        int DoSomething(int a, int b) {
            return a + b;
        }
    }
    int DoSomething(int a, int b) {
        return a - b;
    }
}
namespace NOverload {
    int ProcessTwoArgs(int a, int b) {
        return a+b;
    }
    char ProcessTwoArgs(char a, char b) {
        return std::max(a, b);
    }
    int ProcessTwoArgs(int a, char b) {
        return a - b;
    }
}
namespace NOverflow {
    unsigned int WithOverflow(int a, int b) {
        return a+b;
    }
    uint64_t WithOverflow(int64_t a, int64_t b) {
        return a - b;
    }
}

namespace NLoop {
    int SumInRange(const int lhs, const int rhs){
        long long a = 0;
        for (int i = lhs; i < rhs; i++) {
            a += i;
        }
        return a;
    };
    int CountFixedBitsInRange(const int from, const int to, const int bitsCnt) {
        int counter = 0;
        for (int i = from; i < to; i++) {
            int ch = i;
            int bits = 0;
            while (ch != 0) {
                bits += ch % 2;
                ch /= 2;
            }
            if (bits == bitsCnt) counter++;
        }
        return counter;
    };
}
namespace NMath {
    double ComputeMathFormula(const double a){
        return std::abs((std::sin(a) / 2 + std::cos(a)) * (std::sin(a) / 2 + std::cos(a)) + std::tan(a) * std::atan(a));
    };
    bool IsPositive(int arg){
        if (arg > 0) return true;
        return false;
    };
}

namespace NRecursion {
    int CalculateFibonacci(const int arg){
        if (arg == 0) return 0;
        else if (arg == 1) return 1;
        else return CalculateFibonacci(arg-1) + CalculateFibonacci(arg-2);
    };
}