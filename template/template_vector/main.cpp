#include "task.h"

#if defined(_LIBCPP_VECTOR) || defined(_LIBCPP_LIST) || defined(_LIBCPP_DEQUE) || defined(_LIBCPP_FORWARD_LIST)
#error "include vector, list or deque forbidden"
#else
#include <deque>
#include <forward_list>
#include <list>
#include <vector>
#endif

#include <cassert>
#include <initializer_list>
#include <vector>
#include <iostream>
#include <algorithm>
#include <string>

using namespace std;

template<typename ContainerT>
decltype(declval<ContainerT>().Size()) GetSize(ContainerT&& container) {
    return container.Size();
}

template<typename ContainerT>
decltype(declval<ContainerT>().size()) GetSize(ContainerT&& container) {
    return container.size();
}

template<typename LeftContainerT, typename RightContainerT>
void AssertEqual(LeftContainerT&& lhs, RightContainerT&& rhs) {
    assert(GetSize(lhs) == GetSize(rhs));

    for (size_t i = 0; i < GetSize(lhs); ++i) {
        assert(lhs[i] == rhs[i]);
    }
}

template<typename T>
void TestImplementation(const initializer_list<T>& il, const initializer_list<pair<T, size_t>>& toInsert) {
    vector<T> vec(il);
    Array<T> arr1(cout);

    assert(arr1.Size() == 0);
    assert(arr1.Capacity() == 2);

    for (const auto& value : vec) {
        arr1.PushBack(value);
    }

    AssertEqual(arr1, vec);

    Array<T> arr2(arr1);
    assert(arr1.Capacity() == arr2.Capacity());
    AssertEqual(arr1, arr2);
    assert(arr1 == arr2);

    sort(&arr1[0], &arr1[arr1.Size()]);
    assert(arr1 < arr2);
    sort(&arr1[0], &arr1[arr1.Size()], greater<T>());
    assert(arr1 > arr2);

    size_t insertedCount = 0;
    for (const auto& valPos : toInsert) {
        assert(arr1.Insert(valPos.second, valPos.first));
        ++insertedCount;
        assert(arr1[valPos.second] == valPos.first);
    }
    assert(!arr1.Insert(arr1.Size() + 1, arr1[0]));

    assert(arr1.Size() == arr2.Size() + insertedCount);

    assert(arr1[1] != arr1[0]);
    const T oldFirstElem = arr1[0], oldSecondElem = arr1[1];
    const size_t oldSize = arr1.Size();
    assert(arr1.Erase(0));
    assert(arr1[0] != oldFirstElem);
    assert(arr1[0] == oldSecondElem);
    assert(arr1.Size() == oldSize - 1);
    assert(!arr1.Erase(arr1.Size() + 1));
    assert(arr1.Size() == oldSize - 1);
}

int main() {
    TestImplementation({1, 5, 4, 6, 2, 8}, {{10, 0}, {15, 7}});
    TestImplementation<uint16_t>({1, 5, 4, 6, 2, 8}, {{10, 0}, {15, 7}});
    TestImplementation({1.0f, 5.0f, 4.0f, 6.0f, 2.5f, 8.6f}, {{10.1f, 0}, {15.1f, 7}});
    TestImplementation({1.0, 5.0, 4.0, 6.0, 2.5, 8.6}, {{10.0, 0}, {15.7, 7}});
    TestImplementation<string>({"Ayo", "Ehey", "Doo", "Foo", "Boo", "Hello"}, {{"Ja", 0}, {"Question", 7}});
    return 0;
}
