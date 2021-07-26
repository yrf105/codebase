// https://stackoverflow.com/questions/12993451/c11-stdthread-pooled
// Q：在 C++11 中线程池还必要吗？
// A：必要。C++11 的 std::thread 只是 pthread 的一层封装，
//    当新建一个 std::thread 时
//    就是通过调用 pthread_create
//    来创建一个新的线程，而不会复用之前创建的线程。因此， C++11 中使用
//    std::thread 创建线程和 pthread_create 创建线程的开销相同，依然有
//    必要通过线程池来减小创建线程的开销

#include <pthread.h>

#include <mutex>
#include <thread>
#include <vector>
#include <set>
#include <iostream>

int main(int argc, char** argv) {
    
    std::vector<std::thread> thrs;
    std::set<pthread_t> threadIDs;
    std::mutex mu;

    const int n = 100;

    for (int i = 0; i < n; ++i) {
        thrs.push_back(std::thread([&]{
            std::lock_guard<std::mutex> lock(mu);
            threadIDs.insert(pthread_self());
        }));
    }
    
    for (auto& th : thrs) {
        th.join();
    }

    // 线程 id 各不相同，说明都是新创建的
    std::cout << threadIDs.size() << " " << n << std::endl; // 100 100

    return 0;
}