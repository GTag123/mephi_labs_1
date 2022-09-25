#include "tasks.h"
#include <cmath>

int NOuter::NInner::DoSomething(int lhs, int rhs) {
}

int NOuter::DoSomething(int lhs, int rhs) {
}

int NOverload::ProcessTwoArgs(int lhs, int rhs) {
}

char NOverload::ProcessTwoArgs(char lhs, char rhs) {
}

int NOverload::ProcessTwoArgs(int lhs, char rhs) {
}

unsigned int NOverflow::WithOverflow(int lhs, int rhs) {
}

uint64_t NOverflow::WithOverflow(int64_t lhs, int64_t rhs) {
}

int NLoop::SumInRange(const int lhs, const int rhs) {
}

int NLoop::CountFixedBitsInRange(const int from, const int to, const int bitsCnt) {
}

double NMath::ComputeMathFormula(const double arg) {
}

bool NMath::IsPositive(int arg) {
}

int NRecursion::CalculateFibonacci(const int arg) {
}
