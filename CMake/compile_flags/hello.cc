#include <iostream>
#include <assert.h>

int main() {

#ifdef EX2
    std::cout << "hello compile defination EX2\n";
#endif

#ifdef EX3
    std::cout << "hello compile defination EX3\n";
#endif
    return 0;
}