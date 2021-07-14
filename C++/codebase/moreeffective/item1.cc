#include <iostream>
#include <string>

int main() {

    // char *pc = nullptr; // 设置指针为空值
    // char& rc = *pc; // 引用指向空值

    // error: reference variable "rs" requires an initializer
    // std::string& rs;

    // std::cout << *pc << std::endl;

    std::string s1("yyy");
    std::string s2("rrr");

    std::string *ps = &s1;
    std::string& pr = s1;

    ps = &s2;
    std::cout << *ps << std::endl; // rrr
    std::cout << s1 << std::endl; // yyy
    std::cout << s2 << std::endl; // rrr

    pr = s2;
    std::cout << pr << std::endl; // rrr
    std::cout << s1 << std::endl; // rrr
    std::cout << s2 << std::endl; // rrr

    return 0;
}