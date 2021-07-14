#include <iostream>

int foo() {
    std::cout << "hello\n";
    return 0;
}

int main() {

    using ptrFunc = void(*)(int);

    ptrFunc f = reinterpret_cast<ptrFunc>(foo);

    f(1); // hello

    return 0;
}