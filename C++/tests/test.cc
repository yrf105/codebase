#include <iostream>
#include <functional>
#include <algorithm>
#include <boost/functional/hash/hash.hpp>

struct A {

    A() {
        std::cout << "ctor\n";
    }

    int operator() () {
        std::cout << "()\n";
        return 1;
    }
};


static void test() {
    std::cout << "hello world" << std::endl;

    A a;
    std::cout << typeid(a).name() << "  " << typeid(test).name() << std::endl;
}

static void test_hash_val() {
    A a;
    std::cout << boost::hash_value(&a) << std::endl;
}


struct B {
    int operator<< (int i) {
        return i;
    };
    std::ostream& operator<< (std::ostream& os, int i) {

    }
};



int main(int argc, char** argv) {

    test();
    test_hash_val();

    return 0;
}