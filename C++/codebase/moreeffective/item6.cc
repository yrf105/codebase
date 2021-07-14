#include <iostream>

class INT {

public:

    explicit INT(int i) : i_(i) {}

    INT& operator++() {
        std::cout << "INT& operator++()\n";
        ++i_;
        return *this;
    }

    const INT operator++(int) {
        std::cout << "const INT operator++(int)\n";
        INT tmp = *this;
        ++(*this);
        return tmp;
    }

    INT& operator--() {
        std::cout << "INT& operator--()\n";
        --i_;
        return *this;
    }

    const INT operator--(int) {
        std::cout << "const INT operator--(int)\n";
        INT tmp = *this;
        --(*this);
        return tmp;
    }

private:
    int i_;
};

int main() {

    INT i(5);
    // const INT operator++(int)
    // INT& operator++()
    i++;

    // INT& operator++()
    ++i;

    // INT& operator--()
    --i;

    // const INT operator--(int)
    // INT& operator--()
    i--;

    return 0;
}