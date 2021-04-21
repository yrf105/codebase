#include <functional>
#include <iostream>

static int add(int x, int y) {
    return x + y;
}

auto f = [](int x, int y){ return x + y; };

struct Add {
    int a = 1;

    int add(int x, int y) {
        return x + y + a;
    }

    // 在类内 bind 成员函数
    int add1(int x) {
        return std::bind(&Add::add, this, std::placeholders::_1, 1)(x);
    }

    int operator()(int x, int y) const { return x + y; }
};

int main(int argc, char** argv) {

    // function 封装可调用对象
    auto ADD1 = std::function<int(int, int)>(add);
    auto ADD2 = std::function<int(int, int)>(Add());
    auto ADD3 = std::function<int(int, int)>(f);
    std::function<int(int, int)> f1 = add;
    std::function<int(int, int)> f2 = Add();
    std::function<int(int, int)> f3 = f;

    std::cout << ADD1(1, 2) <<std::endl;
    std::cout << ADD2(1, 2) <<std::endl;
    std::cout << ADD3(1, 2) <<std::endl;
    std::cout << f1(1, 2) <<std::endl;
    std::cout << f2(1, 2) <<std::endl;
    std::cout << f3(1, 2) <<std::endl;

    // bind 可调用对象
    auto add5 = std::bind(add, std::placeholders::_1, 5);
    auto add6 = std::bind(Add(), std::placeholders::_1, 6);
    auto add7 = std::bind(f, std::placeholders::_1, 7);

    std::cout << add5(1) <<std::endl;
    std::cout << add6(1) <<std::endl;
    std::cout << add7(1) <<std::endl;

    // bind 类成员函数
    Add a;
    // 注意：bind 的第一个参数是直接拿类名访问成员函数的
    auto add8 = std::bind(&Add::add, &a, std::placeholders::_1, 8);

    std::cout << add8(8) << std::endl;

    // 在类内 bind 成员函数 
    std::cout << a.add1(8) << std::endl;

    // 给定 bind 模板参数，以改变其返回值类型
    auto add_f = std::bind<float>(f, std::placeholders::_1, std::placeholders::_2);
    std::cout << add_f(1.0, 2.0) << std::endl;

    // 绑定 数据成员
    auto mem = std::bind(&Add::a, &a);
    // 获取成员 a 的值
    std::cout << mem() << std::endl;

    return 0;
}