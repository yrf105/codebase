#include <iostream>

int main() {

    int i = 0, j = 1;

    std::cout << (++i, ++j) << std::endl; // 2

    return 0;
}