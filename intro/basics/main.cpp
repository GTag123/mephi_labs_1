#include "tasks.h"

#include <cassert>
#include <iostream>
#include <limits>
#include <cmath>
#include <iomanip>

void CheckNamespaces() {
    assert(NOuter::NInner::DoSomething(20, 10) == 30);
    assert(NOuter::DoSomething(20, 10) == 10);
}

void CheckOverloadWithBaseTypes() {
    assert(NOverload::ProcessTwoArgs(10, 10) == 20);
    assert(NOverload::ProcessTwoArgs('a', 'b') == 'b');
    assert(NOverload::ProcessTwoArgs('z', 'b') == 'z');
    assert(NOverload::ProcessTwoArgs(100500, 'a') == 100403);
}

void CheckOverflow() {
    assert(NOverflow::WithOverflow(
                std::numeric_limits<int>::max(),
                std::numeric_limits<int>::max()
        ) == std::numeric_limits<unsigned int>::max() - 1);

    assert(NOverflow::WithOverflow(
                std::numeric_limits<int64_t>::min(),
                std::numeric_limits<int64_t>::min()
        ) == std::numeric_limits<uint64_t>::min());
}

void CheckLoop() {
    assert(NLoop::SumInRange(1, 10) == 45);
    assert(NLoop::CountFixedBitsInRange(123321, 2342340, 5) == 17224);
}

void CheckMath() {
    assert(std::fabs(0.8605496856 - NMath::ComputeMathFormula(2.42)) < 1e-9);
    assert(NMath::ComputeMathFormula(0) == 1);
    assert(NMath::IsPositive(10) == true);
    assert(NMath::IsPositive(-1) == false);
}

void CheckRecursion() {
    assert(NRecursion::CalculateFibonacci(40) == 102334155);
}

int main() {
    CheckNamespaces();
    CheckOverloadWithBaseTypes();
    CheckOverflow();
    CheckLoop();
    CheckMath();
    CheckRecursion();
    return 0;
}
