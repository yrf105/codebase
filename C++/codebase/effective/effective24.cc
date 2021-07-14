#include <iostream>

class Rational {
public:
    // 非 explicit 允许隐式转换
    Rational(int n = 0, int d = 1) : n_(n), d_(d) {}
    int n() const { return n_; }
    int d() const { return d_; }
private:
    int n_;
    int d_;
};

const Rational operator*(const Rational& lhs, const Rational& rhs) {
    return Rational(lhs.n() * rhs.n(), lhs.d() + rhs.d());
}

int main() {
    Rational r1(1, 8);
    // Rational r2 = r1 * 2;  // 正确
    // Rational r3 = 2 * r1;  // 正确
}