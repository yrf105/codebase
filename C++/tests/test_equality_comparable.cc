#include <iostream>
#include <inttypes.h>
#include <boost/operators.hpp>

class Number : boost::equality_comparable<Number>{
friend bool operator== (const Number& lhs, const Number& rhs);
public:
    Number(uint64_t n) : n_(n) {}

private:
    uint64_t n_;
};

bool operator== (const Number& lhs, const Number& rhs) {
    return lhs.n_ == rhs.n_;
}

int main(int argc, char** argv) {

    Number n1(1);
    Number n2(2);

    // 若不继承 boost::equality_comparable<Number> 这里的 != 会报错 
    if (n1 != n2) {
        std::cout << "n1 == n2\n";
    }

    return 0;
}