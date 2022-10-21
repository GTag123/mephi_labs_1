#include <iostream>
#include "ostream"
#include "string"

class Array {
public:
    std::ostream& Ostream_;
    std::string getStr(){
        return "Result Array's capacity is " + std::to_string(Capacity_) + "and size is " + std::to_string(Size_);
    }
    Array(std::ostream& ostream):
            Ostream_(ostream){
        Capacity_ = 2;
        Size_ = 0;
        Elements_ = new int[Capacity_];
        Ostream_ << "Constructed. " << getStr() << std::endl;
    };
    Array(const Array& array)
    : Size_(array.Size()),
      Capacity_(array.Capacity()),
      Ostream_(array.Ostream_){
        Elements_ = new int[Capacity_];
        for (int i = 0; i < Size_; ++i) {
            Elements_[i] = array[i];
        }
        Ostream_ << "Constructed from another Array. " << getStr() << std::endl;
    };

    Array(size_t size, std::ostream& ostream = std::cout, int defaultValue = 0)
    : Ostream_(ostream){
        Size_ = size;
        Capacity_ = size * 2;
        Elements_ = new int[Capacity_];
        for (int i = 0; i < size; ++i) {
            Elements_[i] = defaultValue;
        }
        Ostream_ << "Constructed with default. " << getStr() << std::endl;

    };
    ~Array() {
        std::cout << "Destructed " << Size() << std::endl; // add size
    };

    size_t Size() const {
        return Size_;
    };
    size_t Capacity() const {
        return Capacity_;
    };
    void Reserve(size_t newCapacity){
        if (newCapacity <= this->Capacity_) return;
        int* n = new int[newCapacity];
        for (int i = 0; i < this->Size(); ++i) {
            n[i] = (*this)[i];
        }
        delete Elements_;
        Elements_ = n;
    };
    void Resize(size_t newSize){
        if (newSize <= Capacity_){

        } else {
            Reserve(newSize);

        }
        for (int i = Size_; i < newSize; ++i) {
            (*this)[i] = 0;
        }
        Size_ = newSize;
    };
    void PushBack(int value = 0) {
        if (Capacity_ == Size_) {
            Reserve(Capacity_ * 2);
        }
        Elements_[Size_] = value;
        Size_++;
    };

    void PopBack() {
        Size_--;
    };

    const int& operator [](const size_t i) const {
        return Elements_[i];
    };
    int& operator [](const size_t i){
        return Elements_[i];
    };
    explicit operator bool() const{
        if (Size_ > 0) return true;
        return false;
    };

    bool operator <(const Array& it) const{
        for (int i = 0; i < std::min(it.Size(), this->Size_); ++i) {
            if ((*this)[i] < it[i]){
                return true;
            }
            if ((*this)[i] > it[i]){
                return false;
            }
        }
        if (this->Size_ < it.Size()) return true;
        return false;
    };

    bool operator >(const Array& it) const{
        for (int i = 0; i < std::min(it.Size(), this->Size_); ++i) {
            if ((*this)[i] > it[i]){
                return true;
            }
            if ((*this)[i] < it[i]){
                return false;
            }
        }
        if (this->Size_ > it.Size()) return true;
        return false;
    };
    bool operator !=(const Array& it) const{
        return !(*this == it);
    };
    bool operator ==(const Array& it) const{
        if (this->Size_ != it.Size()) return false;
        for (int i = 0; i < this->Size_; ++i) {
            if ((*this)[i] != it[i]) return false;
        }
        return true;
    };
    bool operator <=(const Array& it) const{
        if (*this == it) return true;
        return (*this < it);
    };
    bool operator >=(const Array& it) const{
        if (*this == it) return true;
        return (*this > it);
    };

    Array& operator <<(const int& value){
        PushBack(value);
        return *this;
    };
    Array& operator <<(const Array& it){
        for (int i = 0; i < it.Size(); ++i) {
            PushBack(it[i]);
        }
        return *this;
    };

    friend std::ostream& operator <<(std::ostream& ostream, const Array& array){
        ostream << "Result Array's capacity is " << array.Capacity() << " and size is " <<
                   array.Size() << ", elements are: ";
        for (int i = 0; i < array.Size() - 1; ++i) {
            ostream << array[i] << ", ";
        }
        ostream << array[array.Size() - 1] << std::endl;
        return ostream;
    };

private:
    int* Elements_;
    size_t Size_;
    size_t Capacity_;

};
