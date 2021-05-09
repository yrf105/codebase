#include "STL/List.h"
#include "utils/utils.h"

#include <string>
#include <thread>
#include <functional>
#include <mutex>
#include <list>

int main(int argc, char** argv) {
    
    codebase::List<int> l;
    l.push_back(5);
    l.push_back(6);
    std::cout << l.size() << std::endl;
    std::cout << l.toString() << std::endl;
    codebase::List<int> l2;
    l2 = l;
    l2.push_back(7);
    std::cout << "l: " << l.toString() << std::endl;
    std::cout << "l2: " << l2.toString() << std::endl;

    std::cout << *l.begin() << std::endl;

    return 0;
}

// 内存泄露检测工具 http://senlinzhan.github.io/2017/12/31/valgrind/