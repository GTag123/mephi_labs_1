#include "iostream"
#include "vector"
using namespace std;

class CowString {
private:
    struct Kak;
    Kak* ptr;

    void ptrMinus(){
        (*ptr).addCnt(-1);
        if ((*ptr).countrefs == 0){
            delete ptr;
        }
    }

    void ptrRebase(){
        if (ptr->countrefs > 1){
            Kak* tmp = new Kak();
            tmp->Reserve(ptr->Capacity_);
            tmp->Size_ = ptr->Size_;
            for (int i = 0; i < (int)(*ptr).Size_; ++i) {
                tmp->Elements_[i] = (*ptr).Elements_[i];
            }
            ptrMinus();
            ptr = tmp;
        }
    }

public:
    CowString(){
        ptr = new Kak();
    }

    CowString(CowString& obj){
        ptr = obj.ptr;
        ptr->addCnt(1);
    }

    ~CowString(){
        ptrMinus();
    }


    const char& Back() const{
        return (*ptr).Elements_[(int)(*ptr).Size_-1];
    };

    const char& At(int index) const{
        return (*ptr).Elements_[index];
    };

    char& operator[](int index){
        ptrRebase();
        return (*ptr).Elements_[index];
    };

    CowString& operator=(CowString& rhs){
        ptrMinus();
        this->ptr = rhs.ptr;
        ptr->addCnt(1);
        return *this;
    }


    void PushBack(char value) {
        ptrRebase();
        ptr->PushBack(value);
    };

    void Reserve(size_t newCapacity){
        ptr->Reserve(newCapacity);
    };

    void Resize(size_t newSize){
        ptr->Resize(newSize);
    };


    size_t Size() const{
        return (*ptr).Size_;
    };

    size_t Capacity() const{
        return (*ptr).Capacity_;
    };

private:
    struct Kak{
    public:
        int countrefs;
        size_t Size_ = 0;
        size_t Capacity_ = 2;
        char* Elements_;
        Kak(){
            countrefs = 1;
            Elements_ = new char[2];
        }
        ~Kak(){
            delete[] Elements_;
        }

        void addCnt(int value){
            countrefs += value;
        }
        void PushBack(char value) {
            if (Capacity_ == Size_) {
                Reserve((int)(Capacity_ * 2));
            }
            Elements_[Size_] = value;
            Size_++;
        };
        void Reserve(size_t newCapacity){
            if (newCapacity <= Capacity_) return;
            char* n = new char[newCapacity];
            Capacity_ = newCapacity;
            for (int i = 0; i < (int)Size_; ++i) {
                n[i] = Elements_[i];
            }
            delete[] Elements_;
            Elements_ = n;
        };
        void Resize(size_t newSize){
            if (newSize > Capacity_) {
                Reserve(newSize);
            }
            if (newSize > Size_){
                for (int i = (int)Size_; i < (int)newSize; ++i) {
                    Elements_[i] = 0;
                }
            }
            Size_ = newSize;
        };
    };
};