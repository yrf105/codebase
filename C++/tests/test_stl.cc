#include "STL/utils.h"
#include "random_number/random_number.h"
#include <iostream>

#include <array>
#include <list>
#include <forward_list>
#include <deque>
#include <algorithm>

#include <unistd.h>

static const int ARRAY_TOYAL = 1000000;

static void test_array() {
    codebase::CodeTimer code_timer;
    std::cout << "====== array ======\n";
    code_timer.start();
    std::array<int, ARRAY_TOYAL> a;
    for (int i = 0; i < ARRAY_TOYAL; ++i) {
        a[i] = codebase::rand();
    }
    std::cout << "array 创建花费时间：" << code_timer.stop() << std::endl;

    code_timer.start();
    std::sort(a.begin(), a.end());
    std::cout << "array 排序花费时间：" << code_timer.stop() << std::endl;
}

static void test_forward_list() {
std::cout << "====== forward_list ======\n";
    codebase::CodeTimer code_timer;

    // int sz = codebase::QueryDatatTotal();
    int sz = ARRAY_TOYAL;
    code_timer.start();
    std::forward_list<std::string> l;
    try {
        for (int i = 0; i < sz; ++i) {
            l.push_front(std::to_string(codebase::rand()));
        }
    } catch (std::exception& e) {
        std::cout << e.what();
        abort();
    }
    std::cout << "forward_list 创建花费时间：" << code_timer.stop() << std::endl;

    // std::cout << "forward_list.size() " << l.size() << std::endl;
    std::cout << "forward_list.max_size() " << l.max_size() << std::endl;
    std::cout << "forward_list.front() " << l.front() << std::endl;
    // std::cout << "forward_list.back() " << l.back() << std::endl;

    std::string target = std::to_string(codebase::QueryTarget());
    code_timer.start();
    auto it = std::find(l.begin(), l.end(), target);
    if (it != l.end()) {
        std::cout << "found: " << *it << std::endl;
    } else {
        std::cout << "not found \n";
    }
    std::cout << "find cost: " << code_timer.stop() << std::endl;

    code_timer.start();
    l.sort();
    std::cout << "sort cost: " << code_timer.stop() << std::endl;
}

static void test_list() {
    codebase::CodeTimer code_timer;

    std::cout << "====== list ======\n";
    // int sz = codebase::QueryDatatTotal();
    int sz = ARRAY_TOYAL;

    code_timer.start();
    std::list<std::string> l;
    try {
        for (int i = 0; i < sz; ++i) {
            l.push_back(std::to_string(codebase::rand()));
        }
    } catch (std::exception& e) {
        std::cout << e.what();
        abort();
    }
    std::cout << "list 创建花费时间：" << code_timer.stop() << std::endl;

    std::cout << "list.size() " << l.size() << std::endl;
    std::cout << "list.max_size() " << l.max_size() << std::endl;
    std::cout << "list.front() " << l.front() << std::endl;
    std::cout << "list.back() " << l.back() << std::endl;

    std::string target = std::to_string(codebase::QueryTarget());
    code_timer.start();
    auto it = std::find(l.begin(), l.end(), target);
    if (it != l.end()) {
        std::cout << "found: " << *it << std::endl;
    } else {
        std::cout << "not found \n";
    }
    std::cout << "find cost: " << code_timer.stop() << std::endl;

    code_timer.start();
    l.sort();
    std::cout << "sort cost: " << code_timer.stop() << std::endl;
}

static void test_deque() {
    codebase::CodeTimer code_timer;

    std::cout << "====== deque ======\n";
    // int sz = codebase::QueryDatatTotal();
    int sz = ARRAY_TOYAL;

    code_timer.start();
    std::deque<std::string> dq;
    try {
        for (int i = 0; i < sz; ++i) {
            dq.push_back(std::to_string(codebase::rand()));
        }
    } catch (std::exception& e) {
        std::cout << e.what();
        abort();
    }
    std::cout << "deque 创建花费时间：" << code_timer.stop() << std::endl;

    std::cout << "deque.size() " << dq.size() << std::endl;
    std::cout << "deque.max_size() " << dq.max_size() << std::endl;
    std::cout << "deque.front() " << dq.front() << std::endl;
    std::cout << "deque.back() " << dq.back() << std::endl;

    std::string target = std::to_string(codebase::QueryTarget());
    code_timer.start();
    auto it = std::find(dq.begin(), dq.end(), target);
    if (it != dq.end()) {
        std::cout << "found: " << *it << std::endl;
    } else {
        std::cout << "not found \n";
    }
    std::cout << "find cost: " << code_timer.stop() << std::endl;

    code_timer.start();
    std::sort(dq.begin(), dq.end());
    std::cout << "sort cost: " << code_timer.stop() << std::endl;
}

// stack，queue 是基于 deque 实现的，称为 适配器



int main(int argc, char** argv) {
    // test_array();
    // test_list();
    // test_forward_list();
    test_deque();
    
    return 0;
}
