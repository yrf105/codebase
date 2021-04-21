#include "utils/utils.h"
#include "random_number/random_number.h"
#include <iostream>

#include <array>
#include <vector>
#include <list>
#include <forward_list>
#include <deque>
#include <set>
#include <algorithm>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <typeinfo>

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

static void test_multi_set() {
    codebase::CodeTimer code_timer;

    std::cout << "====== multiset ======\n";
    // int sz = codebase::QueryDatatTotal();
    int sz = ARRAY_TOYAL;

    code_timer.start();
    std::multiset<std::string> s;
    try {
        for (int i = 0; i < sz; ++i) {
            s.insert(std::to_string(codebase::rand()));
        }
    } catch (std::exception& e) {
        std::cout << e.what();
        abort();
    }
    std::cout << "multiset 创建花费时间：" << code_timer.stop() << std::endl;

    std::cout << "multiset.size() " << s.size() << std::endl;
    std::cout << "multiset.max_size() " << s.max_size() << std::endl;
    std::cout << "multiset.begin() " << *(s.begin()) << std::endl;
    // std::cout << "multiset.front() " << s.front() << std::endl;
    // std::cout << "multiset.back() " << s.back() << std::endl;
    

    // std::string target = std::to_string(codebase::QueryTarget());
    std::string target = std::to_string(23456);
    code_timer.start();
    auto it = std::find(s.begin(), s.end(), target);
    if (it != s.end()) {
        std::cout << "found: " << *it << std::endl;
    } else {
        std::cout << "not found \n";
    }
    std::cout << "find cost: " << code_timer.stop() << std::endl;
    
    code_timer.start();
    it = s.find(target);
    if (it != s.end()) {
        std::cout << "found: " << *it << std::endl;
    } else {
        std::cout << "not found \n";
    }
    std::cout << "find cost: " << code_timer.stop() << std::endl;

    // code_timer.start();
    // std::sort(s.begin(), s.end());
    // std::cout << "::sort cost: " << code_timer.stop() << std::endl;
}

static void test_multi_map() {
    codebase::CodeTimer code_timer;

    std::cout << "====== multimap ======\n";
    // int sz = codebase::QueryDatatTotal();
    int sz = ARRAY_TOYAL;

    code_timer.start();
    std::multimap<int, std::string> m;
    try {
        for (int i = 0; i < sz; ++i) {
            // 不能用 []
            m.insert({codebase::rand(), std::to_string(codebase::rand())});
        }
    } catch (std::exception& e) {
        std::cout << e.what();
        abort();
    }
    std::cout << "multimap 创建花费时间：" << code_timer.stop() << std::endl;

    std::cout << "multimap.size() " << m.size() << std::endl;
    std::cout << "multimap.max_size() " << m.max_size() << std::endl;
    std::cout << "multimap.begin() " << m.begin()->second << std::endl;
    // std::cout << "multimap.front() " << s.front() << std::endl;
    // std::cout << "multimap.back() " << s.back() << std::endl;
    

    // std::string target = std::to_string(codebase::QueryTarget());
    // std::string target = std::to_string(23456);
    // code_timer.start();
    // no match for ‘operator==’ (operand types are ‘std::pair<const int, std::__cxx11::basic_string<char> >’ and ‘const int’)
    // 应该需要用户提供比较方法
    // auto it = std::find(m.begin(), m.end(), {23456, "23456"});
    // if (it != m.end()) {
    //     std::cout << "found: " << it->first << std::endl;
    // } else {
    //     std::cout << "not found \n";
    // }
    // std::cout << "find cost: " << code_timer.stop() << std::endl;
    
    code_timer.start();
    auto it = m.find(23456);
    if (it != m.end()) {
        std::cout << "found: " << it->first << std::endl;
    } else {
        std::cout << "not found \n";
    }
    std::cout << "find cost: " << code_timer.stop() << std::endl;

    // code_timer.start();
    // std::sort(s.begin(), s.end());
    // std::cout << "::sort cost: " << code_timer.stop() << std::endl;
}

static void test_unordered_set() {
    codebase::CodeTimer code_timer;

    std::cout << "====== unordered_set ======\n";
    // int sz = codebase::QueryDatatTotal();
    int sz = ARRAY_TOYAL;

    code_timer.start();
    std::unordered_set<std::string> s;
    try {
        for (int i = 0; i < sz; ++i) {
            // 不能用 []
            s.insert(std::to_string(codebase::rand()));
        }
    } catch (std::exception& e) {
        std::cout << e.what();
        abort();
    }
    std::cout << "set 创建花费时间：" << code_timer.stop() << std::endl;

    std::cout << "set.size() " << s.size() << std::endl;
    std::cout << "set.max_size() " << s.max_size() << std::endl;
    std::cout << "set.begin() " << *(s.begin()) << std::endl;
    std::cout << "set.max_bucket_count() " << s.max_bucket_count() << std::endl;
    std::cout << "set.max_load_factor() " << s.max_load_factor() << std::endl;
    // std::cout << "set.front() " << s.front() << std::endl;
    // std::cout << "set.back() " << s.back() << std::endl;
    

    // std::string target = std::to_string(codebase::QueryTarget());
    std::string target = std::to_string(23456);
    code_timer.start();
    auto it = std::find(s.begin(), s.end(), "23456");
    if (it != s.end()) {
        std::cout << "found: " << *it << std::endl;
    } else {
        std::cout << "not found \n";
    }
    std::cout << "find cost: " << code_timer.stop() << std::endl;
    
    code_timer.start();
    it = s.find("23456");
    if (it != s.end()) {
        std::cout << "found: " << *it << std::endl;
    } else {
        std::cout << "not found \n";
    }
    std::cout << "find cost: " << code_timer.stop() << std::endl;

    // code_timer.start();
    // std::sort(s.begin(), s.end());
    // std::cout << "::sort cost: " << code_timer.stop() << std::endl;
}

static void test_unordered_map() {
    codebase::CodeTimer code_timer;

    std::cout << "====== unordered_map ======\n";
    // int sz = codebase::QueryDatatTotal();
    int sz = ARRAY_TOYAL;

    code_timer.start();
    std::unordered_map<int, std::string> m;
    try {
        for (int i = 0; i < sz; ++i) {
            // 不能用 []
            m.insert({codebase::rand(), std::to_string(codebase::rand())});
        }
    } catch (std::exception& e) {
        std::cout << e.what();
        abort();
    }
    std::cout << "unordered_map 创建花费时间：" << code_timer.stop() << std::endl;

    std::cout << "unordered_map.size() " << m.size() << std::endl;
    std::cout << "unordered_map.max_size() " << m.max_size() << std::endl;
    std::cout << "unordered_map.begin() " << m.begin()->second << std::endl;
    std::cout << "unordered_map.max_load_factor() " << m.max_load_factor() << std::endl;
    std::cout << "unordered_map.bucket_count() " << m.bucket_count() << std::endl;
    // std::cout << "unordered_map.front() " << s.front() << std::endl;
    // std::cout << "unordered_map.back() " << s.back() << std::endl;
    

    // std::string target = std::to_string(codebase::QueryTarget());
    // std::string target = std::to_string(23456);
    // code_timer.start();
    // no match for ‘operator==’ (operand types are ‘std::pair<const int, std::__cxx11::basic_string<char> >’ and ‘const int’)
    // 应该需要用户提供比较方法
    // auto it = std::find(m.begin(), m.end(), {23456, "23456"});
    // if (it != m.end()) {
    //     std::cout << "found: " << it->first << std::endl;
    // } else {
    //     std::cout << "not found \n";
    // }
    // std::cout << "find cost: " << code_timer.stop() << std::endl;
    
    code_timer.start();
    auto it = m.find(23456);
    if (it != m.end()) {
        std::cout << "found: " << it->first << std::endl;
    } else {
        std::cout << "not found \n";
    }
    std::cout << "find cost: " << code_timer.stop() << std::endl;

    // code_timer.start();
    // std::sort(s.begin(), s.end());
    // std::cout << "::sort cost: " << code_timer.stop() << std::endl;
}

static void test_set() {
    codebase::CodeTimer code_timer;

    std::cout << "====== set ======\n";
    // int sz = codebase::QueryDatatTotal();
    int sz = ARRAY_TOYAL;

    code_timer.start();
    std::set<std::string> s;
    try {
        for (int i = 0; i < sz; ++i) {
            // 不能用 []
            s.insert(std::to_string(codebase::rand()));
        }
    } catch (std::exception& e) {
        std::cout << e.what();
        abort();
    }
    std::cout << "set 创建花费时间：" << code_timer.stop() << std::endl;

    std::cout << "set.size() " << s.size() << std::endl;
    std::cout << "set.max_size() " << s.max_size() << std::endl;
    std::cout << "set.begin() " << *(s.begin()) << std::endl;
    // std::cout << "set.max_bucket_count() " << s.max_bucket_count() << std::endl;
    // std::cout << "set.max_load_factor() " << s.max_load_factor() << std::endl;
    // std::cout << "set.front() " << s.front() << std::endl;
    // std::cout << "set.back() " << s.back() << std::endl;
    

    // std::string target = std::to_string(codebase::QueryTarget());
    std::string target = std::to_string(23456);
    code_timer.start();
    auto it = std::find(s.begin(), s.end(), "23456");
    if (it != s.end()) {
        std::cout << "found: " << *it << std::endl;
    } else {
        std::cout << "not found \n";
    }
    std::cout << "find cost: " << code_timer.stop() << std::endl;
    
    code_timer.start();
    it = s.find("23456");
    if (it != s.end()) {
        std::cout << "found: " << *it << std::endl;
    } else {
        std::cout << "not found \n";
    }
    std::cout << "find cost: " << code_timer.stop() << std::endl;

    // code_timer.start();
    // std::sort(s.begin(), s.end());
    // std::cout << "::sort cost: " << code_timer.stop() << std::endl;
}

static void test_map() {
    codebase::CodeTimer code_timer;

    std::cout << "====== map ======\n";
    // int sz = codebase::QueryDatatTotal();
    int sz = ARRAY_TOYAL;

    code_timer.start();
    std::map<int, std::string> m;
    try {
        for (int i = 0; i < sz; ++i) {
            // 能用 []
            m[i] = std::to_string(codebase::rand());
        }
    } catch (std::exception& e) {
        std::cout << e.what();
        abort();
    }
    std::cout << "map 创建花费时间：" << code_timer.stop() << std::endl;

    std::cout << "map.size() " << m.size() << std::endl;
    std::cout << "map.max_size() " << m.max_size() << std::endl;
    std::cout << "map.begin() " << m.begin()->second << std::endl;
    // std::cout << "map.max_load_factor() " << m.max_load_factor() << std::endl;
    // std::cout << "map.bucket_count() " << m.bucket_count() << std::endl;
    // std::cout << "map.front() " << s.front() << std::endl;
    // std::cout << "map.back() " << s.back() << std::endl;
    

    // std::string target = std::to_string(codebase::QueryTarget());
    // std::string target = std::to_string(23456);
    // code_timer.start();
    // no match for ‘operator==’ (operand types are ‘std::pair<const int, std::__cxx11::basic_string<char> >’ and ‘const int’)
    // 应该需要用户提供比较方法
    // auto it = std::find(m.begin(), m.end(), {23456, "23456"});
    // if (it != m.end()) {
    //     std::cout << "found: " << it->first << std::endl;
    // } else {
    //     std::cout << "not found \n";
    // }
    // std::cout << "find cost: " << code_timer.stop() << std::endl;
    
    code_timer.start();
    auto it = m.find(23456);
    if (it != m.end()) {
        std::cout << "found: " << it->first << std::endl;
    } else {
        std::cout << "not found \n";
    }
    std::cout << "find cost: " << code_timer.stop() << std::endl;

    // code_timer.start();
    // std::sort(s.begin(), s.end());
    // std::cout << "::sort cost: " << code_timer.stop() << std::endl;
}

static void test_sizeof() {

    std::cout << "=== vector ===" << std::endl;
    std::vector<int> v1;
    std::vector<std::string> v2;
    std::cout << "sizeof(std::vector<int>): " << sizeof(v1) << " sizeof(std::vector<std::string>): " << sizeof(v2) << std::endl; 

    std::cout << "=== list ===" << std::endl;
    std::list<int> l1;
    std::list<std::string> l2;
    std::cout << "sizeof(std::list<int>): " << sizeof(l1) << " sizeof(std::list<std::string>): " << sizeof(l2) << std::endl; 

    std::cout << "=== map ===" << std::endl;
    std::map<int, std::string> m1;
    std::map<std::string, std::vector<std::map<int, int>>> m2;
    std::cout << "sizeof(std::map<int, std::string>)" << sizeof(m1) << " sizeof(std::map<std::string, std::vector<std::map<int, int>>>)" << sizeof(m2) << std::endl;

}

int main(int argc, char** argv) {
    // test_array();
    // test_list();
    // test_forward_list();
    // test_deque();
    // test_multi_set();
    // test_multi_map();
    // test_unordered_set();
    // test_unordered_map();
    // test_set();
    // test_map();

    test_sizeof();

    return 0;
}
