#pragma once
#include "iostream"
#include "vector"
class RIter {
private:
    int64_t stp_, curr_, end_;
public:
    RIter(int64_t step, int64_t current, int64_t end): stp_(step), curr_(current), end_(end){
    };
    int64_t operator *() const{
        return this->curr_;
    }
    RIter& operator++(){

        if(stp_ > 0 && this->curr_ + stp_ > end_){
            this->curr_ = this->end_;
            return *this;
        }
        else if(stp_ < 0 && this->curr_ + stp_ < end_){
            this->curr_ = this->end_;
            return *this;
        }
        this->curr_ += stp_;
        return *this;
    }
    RIter operator++(int){
        RIter old = *this;
        operator++();
        return old;
    }

//    friend RIter operator+(int64_t rhs, RIter& lhs){
//        lhs.curr_ += rhs*lhs.stp_;
//        return lhs;
//    }
    friend bool operator!=(const RIter& it1, const RIter& it2){
        if (it1.curr_ == it2.curr_ && it1.stp_ == it2.stp_){

            return false;
        }
        return true;
    };
    friend bool operator==(const RIter& it1, const RIter& it2){
        if (it1 != it2) return false;
        return true;
    }

};

class Range {
private:
    const long long int start_;
    const long long int end_;
    const long long int step_;
public:
    explicit Range(long long int n):
        start_(0),
        end_(n),
        step_(1){};
    Range(long long int start, long long int end):
        start_(start),
        end_(end),
        step_(1){};
    Range(long long int start, long long int end, long long int step):
            start_(start),
            end_(end),
            step_(step){};

    [[nodiscard]] RIter begin() const {
        return RIter(step_, start_, end_);
    }
    [[nodiscard]] RIter end() const {
        return RIter(step_, end_, end_);
    }

    [[nodiscard]] int64_t Size() const{
        if (!(std::abs(start_ - end_) % std::abs(step_))){
            return std::abs(start_ - end_) / std::abs(step_);
        }
        return std::abs(start_ - end_) / std::abs(step_) + 1;
    }
//    friend bool operator ==(Range& it1, std::vector<int64_t>& it2){
//        std::cout << "st1 " << it1.start_ << ", end1: " << it1.end_ << ", step1: " << it1.step_
//        << std::endl;
//        std::cout << "it2[0] " << it2[0] << ", it2[1]: " << it2[1] << ", it2[1]-[0]: " << it2[1] - it2[0]
//                  << "ending: " << it2[it2.size()-1] << std::endl << std::endl;
//        return true;
//    }

};