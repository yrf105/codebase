#include "STL/String.h"

#include <string>
#include <iostream>
#include <vector>

int main(int argc, char** argv) {

    codebase::String s;
    codebase::String s2("yue");

    s = s2;

    std::cout << s << std::endl;

    return 0;
}