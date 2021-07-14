#include <iostream>
#include <string>

class B {
public:
    std::string i_;
};

class B1 : virtual public B {
public:
    int foo() {
        std::cout << "B1::foo()\n";
        return 0;
    }
private:
    std::string i_;
};

class B2 : virtual public B {
private:
    bool foo() {
        std::cout << "B2::foo()\n";
        return false;
    }
private:
    std::string i_;
};

class D : public B1, public B2 {
private:
    std::string i_;
};

int main() {
    std::cout << sizeof(std::string) << std::endl; // 32
    std::cout << sizeof(D) << std::endl; // 144
    return 0;
}