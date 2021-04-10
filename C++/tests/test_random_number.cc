#include <iostream>

#include "random_number/random_number.h"

int main(int argc, char** argv) {

    std::cout << RAND_MAX << std::endl;

    for (int i = 0; i < 10; ++i) {
        std::cout << codebase::rand() << std::endl;
    }
    
    return 0;
}