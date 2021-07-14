#include <iostream>

template <typename T>
class Rational {
    friend const Rational operator*(const Rational& lhs, const Rational& rhs) {
        return Rational(lhs.n() * rhs.n(), lhs.d() + rhs.d());
    }

public:
    // 非 explicit 允许隐式转换
    Rational(const T& n = 0, const T& d = 1) : n_(n), d_(d) {}
    const T n() const { return n_; }
    const T d() const { return d_; }

private:
    T n_;
    T d_;
};

// template <typename T>
// const Rational<T> operator*(const Rational<T>& lhs, const Rational<T>& rhs) {
//     return Rational<T>(lhs.n() * rhs.n(), lhs.d() + rhs.d());
// }

int main() {
    Rational<int> r1(1, 8);
    // error: no match for ‘operator*’ (operand types are ‘int’ and
    // ‘Rational<int>
    Rational r2 = r1 * 2;  // 错误
    Rational r3 = 2 * r1;  // 错误
    (void)r2;
    (void)r3;
}