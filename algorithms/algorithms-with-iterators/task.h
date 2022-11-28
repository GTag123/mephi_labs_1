#include <cstdlib>
#include <iterator>
#include <iostream>
#include <vector>

using namespace std;

class Iterator {
        friend class FibonacciRange;
        vector<uint64_t>& fib;
        int curr;
    public:
        using value_type = uint64_t;
        using difference_type = ptrdiff_t;
        using pointer = value_type*;
        using reference = value_type&;
        using iterator_category = std::input_iterator_tag;

        Iterator(int pos, vector<uint64_t>& vec): fib(vec), curr(pos){};

        reference operator *() {
            return fib[curr];
        }

        Iterator& operator ++() {
            curr++;
            return *this;
        }

        Iterator operator ++(int) {
            Iterator temp = *this;
            operator++();
            return temp;
        }

        bool operator ==(const Iterator& rhs) const {
            if (curr == rhs.curr && rhs.fib == this->fib){
                return true;
            }
            return false;
        }
        bool operator !=(const Iterator& rhs) const {
            return !(operator==(rhs));
        }

        bool operator <(const Iterator& rhs) const {
            if (this->curr < rhs.curr) return true;
            return false;
        }
        Iterator& operator=(const Iterator& rhs){
            curr = rhs.curr;
            return *this;
        }

    };

class FibonacciRange {
public:
    vector<uint64_t> fib;
    FibonacciRange(size_t amount) {
        fib.push_back(1);
        fib.push_back(2);
        for (int i = 0; i < (int)amount-2; ++i) {
            fib.push_back(fib.back()+fib[(int)fib.size()-2]);
        }
    }

    Iterator begin(){
        return Iterator(0, fib);
    }

    Iterator end(){
        return Iterator((int)fib.size(), fib);
    }

    size_t size() const {
        return fib.size();
    }
};





bool operator !=(vector<uint64_t>::iterator& lhs, Iterator& rhs) {
    return *lhs == *rhs;
}


template<class InputIt, class OutputIt, class UnaryOperation>
void Transform(InputIt firstIn, InputIt lastIn, OutputIt firstOut, UnaryOperation op) {
    while (firstOut != lastIn){
        *firstOut = op(*firstIn);
        firstIn++;
        firstOut++;
    }
}
template<class BidirIt, class UnaryPredicate>
void Partition(BidirIt first, BidirIt last, UnaryPredicate p) {
    stable_partition(first, last, p);
}
template<class InputIt1, class InputIt2, class OutputIt>
void Merge(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2, OutputIt firstOut) {

    for (; first1 < last1 && first2 < last2;) {
        if (*first1 <= *first2){
            *firstOut++ = *first1++;
        } else if(*first1 > *first2) {
            *firstOut++ = *first2++;
        }
    }
    while (first1 < last1) {
        *firstOut++ = *first1++;
    }
    while (first2 < last2) {
        *firstOut++ = *first2++;
    }
}

//class FibonacciRange { не пишите это говно без доппамяти, и вообще делайте что требуют и не выебывайтесь
//public:
//    uint64_t start;
//    uint64_t end_;
//    uint64_t endminus;
//    uint64_t prev;
//    uint64_t curr;
//    size_t sz;
//
//    class Iterator {
//        friend class FibonacciRange;
//        uint64_t start;
//        uint64_t end;
//        uint64_t prev;
//        uint64_t& curr;
//
//    public:
//        using value_type = uint64_t;
//        using difference_type = ptrdiff_t;
//        using pointer = value_type*;
//        using reference = value_type&;
//        using iterator_category = std::input_iterator_tag;
//
//        Iterator(value_type s, value_type en, value_type pre, reference cur):start(s), end(en), prev(pre), curr(cur){};
//
//        reference operator *() const {
//            return curr;
//        }
//
//        Iterator& operator ++() {
//            value_type flo = curr;
//            curr = prev + curr;
//            prev = flo;
//            return *this;
//        }
//
//        Iterator operator ++(int) {
//            Iterator temp = *this;
//            operator++();
//            return temp;
//        }
//
//        bool operator ==(const Iterator& rhs) const {
//            if (start == rhs.start && end == rhs.end && curr == rhs.curr){
//                return true;
//            }
//            return false;
//        }
//        bool operator !=(const Iterator& rhs) const {
//            return !operator==(rhs);
//        }
//
//        bool operator <(const Iterator& rhs) const {
//            if (this->curr < rhs.curr) return true;
//            return false;
//        }
//
//    };
//
//    FibonacciRange(size_t amount) {
//        sz = amount;
//        start = 1;
//        curr = 1;
//        prev = 1;
//        end_ = start;
//        int i = 0, lc = curr, lcprev = prev;
//        for (; i < (int)amount; ++i) {
//            int tmp = lc;
//            lc = lc + lcprev;
//            lcprev = tmp;
//        }
//        end_ = lc;
//        endminus = lcprev;
//    }
//
//    Iterator begin() const {
//        return Iterator(start, end_, start, start);
//    }
//
//    Iterator end() const {
//        return Iterator(start, end_, end_, endminus);
//    }
//
//    size_t size() const {
//        return sz;
//    }
//};
