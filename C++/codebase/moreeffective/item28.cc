#include <iostream>

template <typename T>
class SmartPtr {
public:
    SmartPtr(T* realPtr = nullptr);
    SmartPtr(const SmartPtr& rhs);
    ~SmartPtr();
    SmartPtr& operator=(const SmartPtr& rhs);

    operator bool() const;

    T* operator->() const;
    T& operator*() const;

private:
    T* pointee_;
};

int main() {

    return 0;
}