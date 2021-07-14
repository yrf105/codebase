#include <iostream>

void foo() { // 不指定异常规格的函数，可以抛出任意类型的异常
    std::cout << "foo()\n";
}

// void bar() throw(int) {
//     foo();
// }

int main() {

    return 0;
}