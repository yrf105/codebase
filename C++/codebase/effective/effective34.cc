#include <iostream>

class Base {
public:
    virtual void foo(int i) = 0;
};

void Base::foo(int i) {
    std::cout << "virtual void Base::foo(int i) = 0\n";
}

class Derived : public Base {
public:
    void foo(int i) override {
        std::cout << "void Derived::foo(int i) override\n";
    }
};

int main() {
    
    Derived d;
    d.Base::foo(2); // virtual void Base::foo(int i) = 0
    d.foo(5); // void Derived::foo(int i) override

    return 0;
}