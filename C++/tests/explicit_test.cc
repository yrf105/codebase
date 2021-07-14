#include <iostream>

class A {
public:

    A(int i) : i_(i) {
        std::cout << "调用转换构造函数\n";
    };
    A(int a, int b) : i_(a + b) {}; 

private:
    int i_ = 0;
};

class B {
public:

    explicit B(int i) : i_(i) {};
    explicit B(int a, int b) : i_(a + b) {}; 

private:
    int i_ = 0;
};

void foo(A a){}

A foo1() { return 1; }

int main(int argc, char** argv) {

    // A a1 = 1;
    // A a2 = {1, 2};

    // B b1 = 1; // no suitable constructor exists to convert from "int" to "B"
    // B b2 = {2, 3}; // copy-list-initialization cannot use a constructor marked 'explicit'

    A a1{1};
    a1 = 3;

    foo(1);

    foo1();

    return 0;
}