#include <iostream>

class Array {
public:
    Array(std::ostream& ostream);
    Array(const Array& array);
    Array(size_t size, std::ostream& ostream = std::cout, int defaultValue = 0);
    ~Array();

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
    std::ostream& Ostream_;
};
