#include <tuple>
#include <iostream>
#include <string>
#include <complex>

struct A {
    double b;
    int a;
    std::string c;
    std::complex<double> d;
};

struct B {
    double b;
    int a;
    int e;
    char g;
    int f;
    std::string c;
    std::complex<double> d;
};


int main(int argc, char** argv) {
    std::tuple<std::string, int, int, std::complex<double>> t;

    std::cout << sizeof(int) << std::endl; // 4
    std::cout << sizeof(double) << std::endl; // 8
    std::cout << sizeof(std::string) << std::endl; // 32
    std::cout << sizeof(std::complex<double>) << std::endl; // 16
    std::cout << sizeof(A) << std::endl; // 64
    std::cout << sizeof(B) << std::endl; // 72
    std::cout << sizeof(t) << std::endl; // 56

    auto t2 = std::make_tuple(3, 3.3, "3.33");
    std::cout << std::get<0>(t2) << " " << std::get<1>(t2) << " " << std::get<2>(t2) <<std::endl;  // 3 3.3 3.33

    int a;
    double b;
    std::string s;
    std::tie(a, b, s) = t2;
    std::cout << a << " " << b << " " << s <<std::endl;  // 3 3.3 3.33

    using TupleType = std::tuple<int, std::string>;
    std::cout << std::tuple_size<TupleType>::value << std::endl; // 2

    return 0;
}