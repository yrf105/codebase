#include <iostream>

class INT {
    friend const INT operator+(const INT& lhs, const INT& rhs);
    friend const INT operator+(const INT& lhs, int rhs);
public:
    INT();
    INT(int i) : i_(i) {
        std::cout << "INT(int i) : i_(i)\n";
    }
    INT(const INT& I) : i_(I.i_) {
        std::cout << "INT(const INT& I) : i_(I.i_)\n";
    }

private:
    int i_;
};

const INT operator+(const INT& lhs, const INT& rhs) {
    return INT(lhs.i_ + rhs.i_);
}

const INT operator+(const INT& lhs, int rhs) {
    return INT(lhs.i_ + rhs);
}

int main() {

    INT i1(2);

    // INT(int i) : i_(i)
    // INT(int i) : i_(i)
    // 这里需要调用 2 次 INT(int i) : i_(i)
    // 若不提供 const INT operator+(const INT& lhs, int rhs)
    // 则要调用 3 次 INT(int i) : i_(i)
    INT i2 = i1 + 1;
    (void)i2;

    return 0;
}