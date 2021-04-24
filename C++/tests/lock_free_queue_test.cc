#include "datastructure/lock_free_queue.h"
#include "utils/utils.h"

#include <iostream>
#include <string>
#include <thread>
#include <functional>

static void test() {
    std::cout << " ======test====== \n";

    codebase::LockFreeQueue<std::string> lfq;
    lfq.push("yue");
    lfq.push("rui");
    lfq.push("feng");

    std::cout << lfq.toString() << std::endl;

    lfq.pop();
    lfq.pop();

    std::cout << lfq.toString() << std::endl;

    lfq.pop();
    
    std::cout << lfq.toString() << std::endl;

    lfq.push("yue");
    std::cout << lfq.toString() << std::endl;
}

// int i = 0;

static void test_threads1(codebase::LockFreeQueue<int>* lfq) {
    for (int i = 0; i < 10000; ++i) {
        // std::cout << i << std::endl;
        lfq->push(1);
    }
}

static void test_threads2(codebase::LockFreeQueue<int>* lfq) {
    for (int i = 0; i < 10000; ++i) {
        // std::cout << i << std::endl;
        lfq->push(2);
    }
}

int main(int argc, char** argv) {
    codebase::CodeTimer ct;
    // test();

    codebase::LockFreeQueue<int>* lfq = new codebase::LockFreeQueue<int>;

    ct.start();
    std::thread t1(std::bind(test_threads1, lfq));
    std::thread t2(std::bind(test_threads2, lfq));

    t1.join();
    t2.join();

    std::cout << ct.stop() << std::endl;

    std::cout << lfq->toString() << std::endl;

    return 0;
}