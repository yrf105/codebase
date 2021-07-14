#include <iostream>

class Base {
public:
    void foo(int i) {
        std::cout << "void Base::foo(int i)\n";
    }
    void foo(int a, int b) {
        std::cout << "void Base::foo(int a, int b)\n";
    }
};

class Derived : private Base {
public:
    void foo(int a, int b) { // 转交函数
        Base::foo(a, b); // 自动 inline
    }
    
    void foo() {
        std::cout << "void Derived::foo()\n";
    }
};

int main() {
    
    Derived d;
    d.foo(5, 6);

    return 0;
}