#include <iostream>
#include <string>

class B {
public:
    B(int i = 0) : i_(i) {}
    virtual int i() const {
        return i_;
    }
    virtual ~B() {}

private:
    int i_;
    std::string str_;
};

class D : public B {
public:
    D(int i = 0) : i_(i) {}
    int i() const override {
        return i_;
    }

private:
    int i_;
};

void print(const B bs[], int n) {
    for (int j = 0; j < n; ++j) {
        std::cout << bs[j].i() << std::endl;
    }
}

int main() {

    const int n = 5;
    D ds[n];
    for (int j = 0; j < n; ++j) {
        ds[j] = D(j);
    }

    print(ds, n);

    return 0;
}