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

    size_t Size() const;
    size_t Capacity() const;
    void Reserve(size_t newCapacity);
    void Resize(size_t newSize);
    void PushBack(int value = 0);
    void PopBack();

    const int& operator [](const size_t i) const;
    int& operator [](const size_t i);
    explicit operator bool() const;

    bool operator <(const Array& it) const;
    bool operator >(const Array& it) const;
    bool operator !=(const Array& it) const;
    bool operator ==(const Array& it) const;
    bool operator <=(const Array& it) const;
    bool operator >=(const Array& it) const;

    Array& operator <<(const int& value);
    Array& operator <<(const Array& it);

    friend std::ostream& operator <<(std::ostream& ostream, const Array& array);

private:
    int* Elements_;
    size_t Size_;
    size_t Capacity_;

};
