class CowString {
public:
    // constrcutors
    // copy operator=

    const char& At(size_t index) const;
    char& operator[](size_t index);

    const char& Back() const;

    void PushBack(char c);

    size_t Size() const;
    size_t Capacity() const;

    void Reserve(size_t capacity);
    void Resize(size_t size);
};
