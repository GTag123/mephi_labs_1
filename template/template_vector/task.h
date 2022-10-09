#include <iostream>

template<typename T>
class Array {
public:
    Array(std::ostream& ostream) {
    }

    Array(const Array<T>& array) {
    }

    Array(size_t size, std::ostream& ostream = std::cout, T defaultValue = T()) {
    }

    ~Array() {
    }

    size_t Size() const {
    }

    size_t Capacity() const {
    }

    void Reserve(size_t newCapacity) {
    }

    void Resize(size_t newSize) {
    }

    void PushBack(T value = 0) {
    }

    void PopBack() {
    }

    const T& operator [](const size_t i) const {
    }

    T& operator [](const size_t i) {
    }

    explicit operator bool() const {
    }

    bool operator <(const Array<T>& it) const {
    }

    bool operator >(const Array<T>& it) const {
    }

    bool operator !=(const Array<T>& it) const {
    }

    bool operator ==(const Array<T>& it) const {
    }

    bool operator <=(const Array<T>& it) const {
    }

    bool operator >=(const Array<T>& it) const {
    }

    Array<T>& operator <<(const T& value) {
    }

    Array<T>& operator <<(const Array<T>& it) {
    }

    bool Insert(size_t pos, const T& value) {
    }

    bool Erase(size_t pos) {
    }

private:
    // ToDo
};

template<typename T>
std::ostream& operator <<(std::ostream& ostream, const Array<T>& array) {
}

