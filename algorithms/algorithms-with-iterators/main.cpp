#include "task.h"
#include <cassert>
#include <vector>
#include <algorithm>
#include <functionalu

using namespace std;


void TestPartition(vector<int> vec, std::function<bool(int)> predicate) {
    Partition(vec.begin(), vec.end(), predicate);
    assert(is_partitioned(vec.begin(), vec.end(), predicate));
}

void TestTransform(vector<int> vec, std::function<bool(int)> operation) {
    vector<int> original = vec;

    Transform(vec.begin(), vec.end(), vec.begin(), operation);
    std::transform(original.begin(), original.end(), original.begin(), operation);

    assert(vec == original);
}


void TestMerge(vector<int> vec1, vector<int> vec2) {
    vector<int> result(vec1.size() + vec2.size());
    Merge(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), result.begin());

    vector<int> expectedResult(vec1.size() + vec2.size());
    std::merge(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), expectedResult.begin());
//    cout << result.size() << " " << expectedResult.size() << endl;
//    for (int i = 0; i < (int)result.size(); ++i) {
//        cout << result[i] << " " << expectedResult[i] << endl;
//    }

    assert(result == expectedResult);
}


void TestFibonacciRange() {
    FibonacciRange r(10);

    assert(std::is_sorted(r.begin(), r.end()));

    vector<uint64_t> fibonacciVec(r.begin(), r.end());

    assert(fibonacciVec == vector<uint64_t>({1, 2, 3, 5, 8, 13, 21, 34, 55, 89}));

    vector<uint64_t> doubledFibonacci(r.size());
    Transform(r.begin(), r.end(), doubledFibonacci.begin(), [](uint64_t fib){return fib * 2;});
//    cout << doubledFibonacci.size() << endl;
//    for (int i = 0; i < (int)doubledFibonacci.size(); ++i) {
//        cout << doubledFibonacci[i] << endl;
//    }
    assert(doubledFibonacci == vector<uint64_t>({2, 4, 6, 10, 16, 26, 42, 68, 110, 178}));
}


int main () {
    cout << 123 << endl;
    TestPartition({1, 2, 3, 4, 5, 6, 7, 8}, [](int a){return a % 2;});
    TestPartition({1, 1, 1, 1, 1, 1, 1, 1}, [](int a){return a % 2;});
    TestPartition({2, 2, 2, 2, 1, 1, 1, 1}, [](int a){return a % 2;});
    TestPartition({2, 2, 2, 2, 2, 2, 2, 2}, [](int a){return a % 2;});
    TestPartition({}, [](int a){return a % 2;});

    TestTransform({1, 2, 3, 4, 5, 6, 7, 8}, [](int a){return a % 2;});
    TestTransform({1, 1, 1, 1, 1, 1, 1, 1}, [](int a){return a % 2;});
    TestTransform({2, 2, 2, 2, 1, 1, 1, 1}, [](int a){return a % 2;});
    TestTransform({2, 2, 2, 2, 2, 2, 2, 2}, [](int a){return a % 2;});
    TestTransform({}, [](int a){return a % 2;});

    TestMerge({1, 3, 5, 7}, {2, 4, 6, 8});
    TestMerge({1, 1, 1, 1}, {2, 2, 2, 2});
    TestMerge({1, 2}, {1, 2});
    TestMerge({1, 2}, {});
    TestMerge({}, {1, 2});
    TestMerge({}, {});

    TestFibonacciRange();

    return 0;
}
