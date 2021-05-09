#include "STL/Vector.h"

#include <iostream>

int main(int argc, char** argv) {

    codebase::Vector<int> vec;
    codebase::Vector<int> vec2;
    vec.push_back(5);
    std::cout << vec.capacity() << std::endl;

    vec.push_back(6);
    std::cout << vec.capacity() << std::endl;

    vec.push_back(7);
    std::cout << vec.size() << std::endl;
    std::cout << vec.capacity() << std::endl;

    vec2 = vec;
    vec.push_back(9);

    std::cout << vec.toString() << std::endl;
    std::cout << vec2.toString() << std::endl;

    return 0;
}