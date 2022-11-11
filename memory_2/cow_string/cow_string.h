#include "iostream"
#include "vector"
using namespace std;

struct Kak{
public:
    int countrefs; // хз пока зачем, мб для delete пригодится
    size_t Size_ = 0;
    size_t Capacity_ = 2;
    long long a = 1, b = 2, c = 3, d =5, e = 90, f = 90, x = 99901, pwsd = 102, gar = 89, garage = 29, gashish = 18;
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

class CowString {
    Kak* ptr;
public:
    CowString(){
        ptr = new Kak();
    }

    CowString(CowString& obj){
        ptr = obj.ptr;
        ptr->addCnt(1);
    }
    ~CowString(){
//        cout << "123" << endl;
        ptrminus();
//        cout << "1234" << endl;
    }
    // s
    void ptrminus(){
//        cout << "refsupper: " << (*ptr).countrefs << endl;
        (*ptr).addCnt(-1);
//        cout << "refs: " << (*ptr).countrefs << endl;
        if ((*ptr).countrefs == 0){
//            cout << "trying delete" << endl;
            delete ptr;
        }
    }
    const char& At(int index) const{
//        cout << "index "<< index << ": " << (*ptr).Elements_[index] << endl;
        return (*ptr).Elements_[index];
    };
    char& operator[](int index){
        if (ptr->countrefs > 1){
            Kak* tmp = new Kak();
            tmp->Reserve(ptr->Capacity_);
            tmp->Resize(ptr->Size_); // тута лишнее
            for (int i = 0; i < (int)(*ptr).Size_; ++i) {
//                cout << "Elem in for: " << (*ptr).Elements_[i] << endl;
                tmp->Elements_[i] = (*ptr).Elements_[i];
            }
            ptrminus();
            ptr = tmp;
        }
        return (*ptr).Elements_[index];
    };

    const char& Back() const{
        return (*ptr).Elements_[(int)(*ptr).Size_-1];
    };


    size_t Size() const{
        return (*ptr).Size_;
    };
    size_t Capacity() const{
        return (*ptr).Capacity_;
    };


    void PushBack(char value) {
        if (ptr->countrefs > 1){
//            cout << "FFF" << endl;
            Kak* tmp = new Kak();
            tmp->Reserve(ptr->Capacity_);
            tmp->Resize(ptr->Size_); // тута лишнее
            for (int i = 0; i < (int)(*ptr).Size_; ++i) {
                tmp->Elements_[i] = (*ptr).Elements_[i];
            }
            ptrminus();
            ptr = tmp;
        }
        ptr->PushBack(value);
    };
    void Reserve(size_t newCapacity){
        ptr->Reserve(newCapacity);
    };
    void Resize(size_t newSize){
        ptr->Resize(newSize);
    };
    CowString& operator=(CowString& rhs){
        ptrminus();
        this->ptr = rhs.ptr;
        ptr->addCnt(1);
        return *this;
    }


};