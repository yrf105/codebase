#include <iostream>
#include <vector>

class Base {
public:
    Base(int i = 0) : i_(i) {}
    virtual ~Base() {}
    virtual void derivedFoo() = 0;
private:
    int i_;
};

class Derived : public Base {
public:
    Derived(int i = 0) : i_(i) {}
    void derivedFoo() override {
        std::cout << "derivedFoo " << i_ << std::endl;
    }
private:
    int i_;
};

class Derived1 : public Base {
public:
    Derived1(int i = 0) : i_(i) {}
    void derivedFoo() override {
        std::cout << "derivedFoo " << i_ << std::endl;
    }
private:
    int i_;
};

int main() {
    Derived d1(1);
    Derived d2(2);
    Derived1 d1_1(1);
    Derived1 d1_2(2);
    std::vector<Base*> vec{&d1, &d2, &d1_1, &d1_2};
    for (auto derived : vec) {
        if (Derived* d = dynamic_cast<Derived*>(derived)) {
            d->derivedFoo();
        } else if (Derived1* d1 = dynamic_cast<Derived1*>(derived)) {
            d1->derivedFoo();
        }
    } 

    return 0;
}