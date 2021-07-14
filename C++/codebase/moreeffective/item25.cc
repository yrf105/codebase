#include <iostream>
#include <string>

class B {
public:
    virtual ~B() {};

    void setStr(const std::string& str) {
        str_ = str;
    }

    std::string str() const {
        return str_;
    }

    virtual B* clone() {
        return new B(*this);
    }

    virtual std::string info() const {
        return "B";
    }

private:
    std::string str_;
};

class D1 : public B {
public:
    virtual ~D1() {};

    virtual D1* clone() override {
        return new D1(*this);
    }

    virtual std::string info() const override {
        return "D1";
    }

private:
};

class D2 : public B {
public:
    virtual ~D2() {};

    virtual D2* clone() override {
        return new D2(*this);
    }

    virtual std::string info() const override {
        return "D2";
    }

private:
};

std::ostream& operator<<(std::ostream& os, const B& b) {
    return os << b.info();
}

int main() {

    D1 d1;
    D2 d2;
    d1.setStr("d1");
    d2.setStr("d2");

    B* b1 = &d1;
    B* b2 = &d2;

    std::cout << b1->str() << std::endl;
    std::cout << b2->str() << std::endl;

    B* d1Clone = b1->clone();
    B* d2Clone = b2->clone();

    std::cout << d1Clone->str() << std::endl;
    std::cout << d2Clone->str() << std::endl;

    std::cout << *b1 << std::endl;
    std::cout << *b2 << std::endl;

    delete d1Clone;
    delete d2Clone;

    return 0;
}