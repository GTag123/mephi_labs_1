#pragma once
#include "iostream"
#include "vector"
#include "string"
using namespace std;

template<typename T>
class FlatIter {
private:
    int pos_;
    vector<vector<T>>& data_;
    vector<int>& prefixsum_;
public:
    using iterator_category = random_access_iterator_tag;
    using value_type = T;
    using reference = T&;
    using pointer = T*;
    using difference_type = ssize_t;

    FlatIter<T>(vector<vector<T>>& data, int pos, vector<int>& ps): pos_(pos), data_(data), prefixsum_(ps){};
//    ~FlatIter(){
//        delete &prefixsum_;
//    }
    T& operator *() const{
        auto outerPos = std::upper_bound(prefixsum_.begin(), prefixsum_.end(), pos_) - 1;
        long long innerPos = pos_ - *outerPos;

        return data_[outerPos - prefixsum_.begin()][innerPos];

//        auto it = upper_bound(prefixsum_.begin(), prefixsum_.end(), pos_) - 1;
//        int id = distance(prefixsum_.begin(), it);
//
//        if (this->pos_ == 42) {
//            cout << "42 *: " <<  data_[id][pos_ - prefixsum_[id]] << endl;
//        }
//
//        return data_[id][pos_ - prefixsum_[id]];
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
//        T* n = new T; // попробовать T(this->pos_ + diff);
//        *n = *this + ;
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
    FlattenedVector(vector<vector<T>>& vec): vec_(vec){
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
    vector<int> prefixsum_;
    vector<vector<T>>& vec_;
};
