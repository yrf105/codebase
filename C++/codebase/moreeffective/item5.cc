#include <iostream>
#include <string>

class Name {
public:
    Name(const std::string& name) : name_(name) {
        std::cout << "Name(const string& name) : name_(name)\n";
    }

    std::string name() const { return name_; }

private:
    std::string name_;
};

class Rational {
public:
    Rational(int a = 0, int b = 1) : a_(a), b_(b) {
        std::cout << "Rational(int a_ = 0, int b_ = 1) : a_(a), b_(b)\n";
    }

    operator double() const {
        std::cout << "double operator() const\n";
        return static_cast<double>(a_) / b_;
    }

private:
    int a_;
    int b_;
};

void printName(const Name& name) { std::cout << name.name() << std::endl; }

int main() {
    std::string name = "yrf";
    // 调用 Name(const std::string& name) 完成隐式类型转换
    printName(name); // yrf

    Rational a(1, 2);
    // 调用 double operator() const 将 a 隐式转换为 double
    double b = a * 0.5;
    std::cout << b << std::endl; // 0.25
    return 0;
}