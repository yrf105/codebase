#include "STL/smart_ptr.h"

#include <iostream>
#include <memory>
#include <string>

int main(int argc, char** argv) {

    // std::shared_ptr<int> p_i = std::shared_ptr<int>(new int(9));
    // std::cout << p_i.use_count() << std::endl;

    codebase::smart_ptr<int> p_i = codebase::smart_ptr<int>(2);
    p_i = p_i;
    std::cout << *p_i << std::endl;
    std::cout << p_i.use_count() << std::endl;

    codebase::smart_ptr<int> p_i2 = p_i;
    std::cout << *p_i2 << std::endl;
    std::cout << p_i2.use_count() << std::endl;

    *p_i = 9;

    codebase::smart_ptr<int> p_i3 = p_i2;
    std::cout << *p_i3 << std::endl;
    std::cout << p_i3.use_count() << std::endl;

    p_i3 = p_i;
    std::cout << *p_i3 << std::endl;
    std::cout << p_i3.use_count() << std::endl;
    std::cout << p_i.use_count() << std::endl;
    std::cout << p_i2.use_count() << std::endl;

    codebase::smart_ptr<std::string> str_smp = codebase::smart_ptr<std::string>::make_smart("yue");
    std::cout << *str_smp << std::endl;

    return 0;
}