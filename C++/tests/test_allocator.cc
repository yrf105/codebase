#include <list>
#include <ext/array_allocator.h>
#include <ext/bitmap_allocator.h>
#include <ext/pool_allocator.h>
#include <memory>
#include <iostream>

#include "utils/utils.h"

// static void test_array_allocator() {
//     codebase::CodeTimer code_timer;
//     std::cout << "====== array_allocator ======\n";
//     code_timer.start();
//     std::list<int, __gnu_cxx::array_allocator<int>> l;
//     for (int i = 0; i < 5000000; ++i) {
//         l.push_back(i);
//     }
//     std::cout << "耗时：" << code_timer.stop() << std::endl;
// }

static void test_bitmap_allocator() {
    codebase::CodeTimer code_timer;
    std::cout << "====== bitmap_allocator ======\n";
    code_timer.start();
    std::list<int, __gnu_cxx::bitmap_allocator<int>> l;
    for (int i = 0; i < 5000000; ++i) {
        l.push_back(i);
    }
    std::cout << "耗时：" << code_timer.stop() << std::endl;
}

static void test_pool_allocator() {
    codebase::CodeTimer code_timer;
    std::cout << "====== pool_allocator ======\n";
    code_timer.start();
    std::list<int, __gnu_cxx::__pool_alloc<int>> l;
    for (int i = 0; i < 5000000; ++i) {
        l.push_back(i);
    }
    std::cout << "耗时：" << code_timer.stop() << std::endl;
}

static void test_allocator() {
    codebase::CodeTimer code_timer;
    std::cout << "====== allocator ======\n";
    code_timer.start();
    std::list<int> l;
    for (int i = 0; i < 5000000; ++i) {
        l.push_back(i);
    }
    std::cout << "耗时：" << code_timer.stop() << std::endl;
}

int main(int argc, char** argv) {

    // test_array_allocator();
    test_bitmap_allocator();
    test_pool_allocator();
    test_allocator();
    return 0;
}