#include <iostream>

struct A {
    char c;       // 内存位置 #1
    int i1 : 5;   // 内存位置 #2
    int i2 : 11,  // 内存位置 #2
        : 0;      // 无名位域，指定类定义中下一个位域将始于分配单元的边界
    int i3 : 8;   // 内存位置 #3
    struct {
        int i4 : 5;  // 内存位置 #4
    };
};

int main(int argc, char** argv) {
    std::cout << sizeof(A) << std::endl;  // 12
    return 0;
}