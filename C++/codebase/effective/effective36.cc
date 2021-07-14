#include <iostream>

class Base {
public:
    void foo() {
        std::cout << "Base::foo()\n";
    }
};

class Derived : public Base {
public:
    void foo() {
        std::cout << "Derived::foo()\n";
    }
};

int main() {
    Derived d;
    Derived* dPtr = &d;
    Base* bPtr = &d;

    dPtr->foo(); // Derived::foo()
    bPtr->foo(); // Base::foo()

    return 0;
}