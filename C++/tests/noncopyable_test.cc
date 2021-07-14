#include "utils/noncopyable.h"

#include <iostream>

class W : public codebase::Noncopyable {
public:
    virtual void print() { std::cout << "W\n"; }
};

class C {
public:
    

};

int main(int argc, char** argv) {

    W w;
    w.print();

    C c1;
    C c2;
    c1 = c2;  

    return 0;
}