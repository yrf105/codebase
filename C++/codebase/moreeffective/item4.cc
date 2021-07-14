#include <iostream>
#include <string>
#include <vector>

class A {
public:
    A(const std::string& a) : a_(a) {}

private:
    std::string a_;
};

int main() {
    const int n = 5;
    // A as[n]; // error: no default constructor exists for class "A"
    // A* as = new A[n]; // error: 同上

    // 解决方法 1
    // 使用非堆数组并在定义时提供参数
    A as[n] = {
        A("1"), A("2"), A("3"), A("4"), A("5"),
    };

    // 这样也可以通过编译
    A* as2 = new A[n]{
        A("1"), A("2"), A("3"), A("4"), A("5"),
    };

    (void)as;
    (void)as2;

    // 解决方法 2
    // 使用指针数组代替对象数组
    A* pas[n];  // 正确
    (void)pas;
    // 缺点：
    // 1. 造成多余的内存分配
    //    - 可以先分配足够大的 raw memory
    //    - 再使用 placement new 解决该问题
    // 2. 更易造成内存泄露

    std::vector<A> veca;

    return 0;
}