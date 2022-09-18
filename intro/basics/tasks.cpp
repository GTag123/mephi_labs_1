#include "tasks.h"
#include <cmath>

int NOuter::NInner::DoSomething(int lhs, int rhs) {
    return lhs + rhs;
}

int NOuter::DoSomething(int lhs, int rhs) {
    return lhs - rhs;
}

int NOverload::ProcessTwoArgs(int lhs, int rhs) {
    return lhs + rhs;
}

char NOverload::ProcessTwoArgs(char lhs, char rhs) {
    return lhs > rhs ? lhs : rhs;
}

int NOverload::ProcessTwoArgs(int lhs, char rhs) {
    return lhs - (int) rhs;
}

unsigned int NOverflow::WithOverflow(int lhs, int rhs) {
    return (unsigned int) lhs + (unsigned int) rhs;
}

uint64_t NOverflow::WithOverflow(int64_t lhs, int64_t rhs) {
    return (uint64_t) lhs - (uint64_t) rhs;
}

int NLoop::SumInRange(const int lhs, const int rhs) {
    int res = 0;
    for (int i = lhs; i < rhs; ++i) {
        res += i;
    }
    return res;
}

int NLoop::CountFixedBitsInRange(const int from, const int to, const int bitsCnt) {
    int res = 0;
    for (int i = from; i < to; ++i) {
        int current = 0;
        for (int b = 0; b < 32; ++b) {
            current += ((i >> b) & 1);
        }
        res += (current == bitsCnt);
    }
    return res;
}

double NMath::ComputeMathFormula(const double arg) {
    if (arg == 0) {
        return -1;
    }
    double tmp = std::sin(arg) / 2 + std::cos(arg);
    tmp *= tmp;
    return std::fabs(tmp + std::tan(arg) * std::atan(arg));
}

bool NMath::IsPositive(int arg) {
    return arg > 0;
}

int NRecursion::CalculateFibonacci(const int arg) {
    if (arg == 0) {
        return 0;
    } else if (arg == 1) {
        return 1;
    }
    return NRecursion::CalculateFibonacci(arg - 1) + NRecursion::CalculateFibonacci(arg - 2);
}
