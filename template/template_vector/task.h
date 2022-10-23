#include <iostream>

template<typename T>
class Array {
public:
    Array(std::ostream& ostream):
            Ostream_(ostream){
        Capacity_ = 2;
        Size_ = 0;
        Elements_ = new T[Capacity_];
        Ostream_.clear();
        Ostream_ << "Constructed. " << (*this);
    };
    Array(const Array& array)
            : Size_(array.Size()),
              Capacity_(array.Capacity()),
              Ostream_(array.Ostream_){
        Elements_ = new T[Capacity_];

        for (int i = 0; i < (int)Size_; ++i) {
            Elements_[i] = array[i];
        }
        Ostream_.clear();
        Ostream_ << "Constructed from another Array. " << (*this) << std::endl;
    };
    Array(size_t size, std::ostream& ostream = std::cout, T defaultValue = T()): Ostream_(ostream) {
        Size_ = size;
        Capacity_ = size * 2;
        Elements_ = new T[Capacity_];
        for (int i = 0; i < size; ++i) {
            Elements_[i] = defaultValue;
        }
        Ostream_.clear();
        Ostream_ << "Constructed with default. " << (*this) << std::endl;
    };

    ~Array() {
        delete[] Elements_;
        Ostream_.clear();
        Ostream_ << "Destructed " << Size() << std::endl; // add size
    };

    size_t Size() const {
        return Size_;
    };
    size_t Capacity() const {
        return Capacity_;
    };

    void Reserve(size_t newCapacity){
        if (newCapacity <= this->Capacity_) return;
        T* n = new T[newCapacity];
        Capacity_ = newCapacity;
        for (int i = 0; i < (int)this->Size(); ++i) {
            n[i] = (*this)[i];
        }
        delete []Elements_;
        Elements_ = n;
    };
    void Resize(size_t newSize){
        if (newSize > Capacity_) {
            Reserve(newSize);
        }
        if (newSize > Size_){
            for (int i = (int)Size_; i < newSize; ++i) {
                (*this)[i] = 0;
            }
        }
        Size_ = newSize;
    };
    void PushBack(T value = 0) {
        if (Capacity_ == Size_) {
            Reserve(Capacity_ * 2);
        }
        Elements_[Size_] = value;
        Size_++;
    };

    void PopBack() {
        Size_--;
    };

    const T& operator [](const size_t i) const {
        return Elements_[i];
    }

    T& operator [](const size_t i) {
        return Elements_[i];
    }

    explicit operator bool() const {
        if ((int)Size_ > 0) return true;
        return false;
    }

    bool operator <(const Array<T>& it) const {
        for (int i = 0; i < std::min( (int)it.Size(), (int)this->Size_); ++i) {
            if ((*this)[i] < it[i]){
                return true;
            }
            if ((*this)[i] > it[i]){
                return false;
            }
        }
        if (this->Size_ < it.Size()) return true;
        return false;
    }

    bool operator >(const Array<T>& it) const {
        for (int i = 0; i < std::min( (int)it.Size(), (int)this->Size_); ++i) {
            if ((*this)[i] > it[i]){
                return true;
            }
            if ((*this)[i] < it[i]){
                return false;
            }
        }
        if (this->Size_ > it.Size()) return true;
        return false;
    }

    bool operator !=(const Array<T>& it) const {
        return *this != it;
    }

    bool operator ==(const Array<T>& it) const {
        if (this->Size_ != it.Size()) return false;
        for (int i = 0; i < (int)this->Size_; ++i) {
            if ((*this)[i] != it[i]) return false;
        }
        return true;
    }

    bool operator <=(const Array<T>& it) const {
        if (*this == it) return true;
        return (*this < it);
    }

    bool operator >=(const Array<T>& it) const {
        if (*this == it) return true;
        return (*this > it);
    }

    Array<T>& operator <<(const T& value) {
        PushBack(value);
        return *this;
    }

    Array<T>& operator <<(const Array<T>& it) {
        for (int i = 0; i < (int)it.Size(); ++i) {
            PushBack(it[i]);
        }
        return *this;
    }

    bool Insert(size_t pos, const T& value) {
        if (pos > Size()) return false;
        if (pos == Size_) {
            PushBack(value);
            return true;
        }
        if (Capacity() <= Size()) {
            Reserve(Capacity() * 2);
        }
        T* n = new T[Capacity()];
        for (int i = 0; i < (int)pos; ++i) {
            n[i] = (*this)[i];
        }
        n[(int)pos] = value;
        for (int i = (int)pos + 1; i < (int)Size() + 1; ++i) {
            n[i] = (*this)[i-1];
        }
        delete[] Elements_;
        Elements_ = n;
        Size_++;
//        std::cout << "open debug" << std::endl;
//        for (int i = 0; i < (int)Size_; ++i) {
//            std::cout << (*this)[i] << " ";
//        }
//        std::cout << "\nclose debug" << std::endl;
        return true;
    }

    bool Erase(size_t pos) {
        if (pos >= Size()) return false;
        for (int i = (int)pos; i < (int)Size(); ++i) {
            (*this)[i] = (*this)[i+1];
        }
        Size_--;
        return true;
    }

private:
    T* Elements_;
    size_t Size_;
    size_t Capacity_;
    std::ostream& Ostream_;
};

template<typename T>
std::ostream& operator <<(std::ostream& ostream, const Array<T>& array) {
    ostream.clear();

    ostream << "Result Array's capacity is " << array.Capacity() << " and size is " <<
            array.Size();

    if((int)array.Size() > 0){
        ostream << ", elements are: ";
        for (int i = 0; i < (int)array.Size() - 1; ++i) {
            ostream << array[i] << ", ";
        }
        ostream << array[(int)array.Size() - 1];
    }

    return ostream;
}

