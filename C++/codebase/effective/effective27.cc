#include <iostream>

class Base {
public:
    Base(int i = 0) : i_(i) {}
    virtual ~Base() {}

    virtual void changeI(int i) {
        i_ = i;
    }

    int getMem() const {
        return i_;
    }

private:
    int i_;
};

class Derived : public Base {
public:
    Derived() : i_(0) {}

    void changeI(int i) override {
        Base::changeI(i);
        i_ = i;
    }

    int getI() const {
        return i_;
    }

private:
    int i_;
};

int main() {

    Derived d;
    d.changeI(5);
    std::cout << d.getMem() << std::endl; // 5
    std::cout << d.getI() << std::endl; // 5

    return 0;
}