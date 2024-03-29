#pragma once
#include "iostream"
#include "vector"
#include "string"
#include "algorithm"
//using namespace std;

template<typename T>
class FlatIter {
private:
    int pos_;
    std::vector<std::vector<T>>& data_;
    std::vector<int>& prefixsum_;
public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using reference = T&;
    using pointer = T*;
    using difference_type = ssize_t;

    FlatIter<T>(std::vector<std::vector<T>>& data, int pos, std::vector<int>& ps): pos_(pos), data_(data), prefixsum_(ps){};
//    ~FlatIter(){
//        delete &prefixsum_;
//    }
    reference operator *() const{
        int l = -1;
        int r = (int) prefixsum_.size();
        while (r-l > 1){
            int m = (r+l)/2;
            if (prefixsum_[m] > pos_){
                r = m;
            } else {
                l = m;
            }
        }
        auto it = prefixsum_.begin() + r - 1;

//        auto it = std::upper_bound(prefixsum_.begin(), prefixsum_.end(), pos_) - 1;
        int id = distance(prefixsum_.begin(), it);

        return data_[id][pos_ - prefixsum_[id]];
    }
    bool operator ==(const FlatIter<T>& it2) const{
        if (this->pos_ == it2.pos_ && this->data_.size() == it2.data_.size() &&
            this->prefixsum_[(int) prefixsum_.size() - 1] == it2.prefixsum_[(int) prefixsum_.size() - 1] &&
            (int) this->prefixsum_.size() == (int) it2.prefixsum_.size()){
            return true;
        }
        return false;
    }

    bool operator !=(const FlatIter<T>& it2) const{
        if (*this == it2) return false;
        return true;
    }

    FlatIter<T>& operator +=(int value) {
        this->pos_ += value;
        return *this;
    }

    FlatIter<T>& operator -=(int value) {
        this->pos_ -= value;
        return *this;
    }

    FlatIter<T>& operator ++(){
        this->pos_++;
        return *this;
    }
    FlatIter<T> operator ++(int){
        T old = *this;
        operator++();
        return old;
    }
    FlatIter<T>& operator --(){
        this->pos_--;
        return *this;
    }
    FlatIter<T> operator --(int){
        T old = *this;
        operator--();
        return old;
    }
    T& operator[](int diff) const{
        return *(*this + diff);
    }



    bool operator <= (const FlatIter<T>& rhs) const{
        if (this->pos_ > rhs.pos_) return false;
        return true;
    }
    bool operator > (const FlatIter<T>& rhs) const{
        if (this->operator<=(rhs)) return false;
        return true;
    }
    bool operator < (const FlatIter<T>& rhs) const{
        if (this->pos_ < rhs.pos_) return true;
        return false;
    }

    FlatIter<T>& operator=(FlatIter<T>& rhs){
        this->pos_ = rhs.pos_;
        this->data_ = rhs.data_;
        this->prefixsum_ = rhs.prefixsum_;
        return *this;
    }


    FlatIter<T> operator +(int value) const{ // попробовать убрать конст какая будет ошибка?
        if (value + this->pos_ > prefixsum_.back()) {
            return FlatIter<T>(this->data_, (int) prefixsum_.back(), prefixsum_);
        }
        FlatIter<T> ret = FlatIter<T>(this->data_, this->pos_ + value, prefixsum_);
//        cout << "operator + int: " << ret.pos_ << " thispos: " << this->pos_ << endl;
        return ret;
    }
    friend FlatIter<T> operator +(const int value, const FlatIter<T>& rhs) {
        return rhs + value;
    }

    FlatIter<T> operator -(int value) const{
        if (value + this->pos_ < 0) {
            return FlatIter(this->data_, 0, prefixsum_);
        }
        return FlatIter<T>(this->data_, this->pos_ - value, prefixsum_);
    }
    friend FlatIter<T> operator -(int value, FlatIter<T>& rhs) {
        return rhs - value;
    }
    int operator -(const FlatIter<T> it2) const{ // попробовать убрать конст какая будет ошибка?
//        cout << "operator - int: " <<  this->pos_ << " " << it2.pos_ << endl;
        return this->pos_ - it2.pos_;
    }
//    int end() {
//        return prefixsum_[(int) prefixsum_.size() - 1];
//    }

};

template<typename T>
class FlattenedVector {
public:
    FlattenedVector(std::vector<std::vector<T>>& vec): vec_(vec){
        prefixsum_.push_back(0);
        for (int i = 1; i < (int) vec_.size() + 1; ++i) {
            prefixsum_.push_back(prefixsum_[(int)prefixsum_.size() - 1] + (int) vec_[i-1].size());
        }
    };
//    ~FlattenedVector() {
//        delete psum;
//    }
    FlatIter<T> begin() {
        return FlatIter<T>(vec_, 0, prefixsum_);
    }
    FlatIter<T> end() {
        return FlatIter<T>(vec_, prefixsum_.back(), prefixsum_);
    }

private:
    std::vector<int> prefixsum_;
    std::vector<std::vector<T>>& vec_;
};