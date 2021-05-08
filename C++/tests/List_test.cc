#include "STL/List.h"
#include "utils/utils.h"

#include <string>
#include <thread>
#include <functional>
#include <mutex>

static void test1() {
    codebase::List<std::string> l;
    l.push_back("2");
    l.push_back("3");
    l.push_front("0");
    l.push_front("1");

    std::cout << "size: " << l.size() << std::endl;
    std::cout << "toString: " << l.toString() << std::endl;

    l.pop_back();
    l.pop_front();
    std::cout << "size: " << l.size() << std::endl;
    std::cout << "toString: " << l.toString() << std::endl;   
    
    l.clear();
    std::cout << "size: " << l.size() << std::endl;
    std::cout << "toString: " << l.toString() << std::endl;   

    l.push_front("cc");
    std::cout << "size: " << l.size() << std::endl;
    std::cout << "toString: " << l.toString() << std::endl;   
}

static void test() {
    codebase::List<int> l;
    l.push_back(2);
    l.push_back(3);
    l.push_front(0);
    l.push_front(1);

    std::cout << "size: " << l.size() << std::endl;
    std::cout << "toString: " << l.toString() << std::endl;

    l.pop_back();
    l.pop_front();
    l.pop_back();
    l.pop_front();
    std::cout << "size: " << l.size() << std::endl;
    std::cout << "toString: " << l.toString() << std::endl;    
}

// int i = 0;
std::mutex mu;

static void test_threads1(codebase::List<int>* l) {
    for (int i = 0; i < 10000; ++i) {
        mu.lock();
        l->push_back(1);
        mu.unlock();
    }
}

static void test_threads2(codebase::List<int>* l) {
    for (int i = 0; i < 10000; ++i) {
        mu.lock();
        l->push_back(2);
        mu.unlock();
    }
}

static void test_swap() {
    codebase::List<std::string> l1;
    l1.push_back("1");
    l1.push_back("2");
    codebase::List<std::string> l2;
    l2.push_back("!");
    l2.push_back("@");
    l1.swap(l2);

    std::cout << l1.toString() << std::endl;
    std::cout << l2.toString() << std::endl;

    l1 = l2;
    l1.push_back("3");
    std::cout << l1.toString() << std::endl;
    std::cout << l2.toString() << std::endl;
}

int main(int argc, char** argv) {
    // codebase::CodeTimer ct;
    // // test();
    // // test1();
    // codebase::List<int>* l = new codebase::List<int>;
    // ct.start();

    // std::thread t1(std::bind(test_threads1, l));
    // std::thread t2(std::bind(test_threads2, l));
    // t1.join();
    // t2.join();

    // std::cout << ct.stop() << std::endl;

    // std::cout << l->toString() << std::endl;

    test_swap();

    return 0;
}

// 内存泄露检测工具 http://senlinzhan.github.io/2017/12/31/valgrind/