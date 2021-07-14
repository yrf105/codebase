#include <iostream>

class B {
public:
    virtual void foo(char c = 'B') {
        std::cout << c << std::endl;
        std::cout << "B::foo()\n";
    }
};

class D : public B {
public:
    void foo(char c = 'C') override {
        std::cout << c << std::endl;
        std::cout << "D::foo()\n";
    }
};

int main() {
    D d;
    B* b = &d;
    // b 的静态类型为 B*，动态类型为 D*
    b->foo();
    // B
    // D::foo()
}