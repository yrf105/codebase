#include <readerwriterqueue.h>

#include <chrono>
#include <functional>
#include <future>
#include <iostream>
#include <thread>

extern void doSomething();

using calls_p_second_t = long long unsigned int;
using the_call_t = std::function<void()>;

template <typename T>
calls_p_second_t testFunc(const T& func, int internal) {
    auto now = []() -> auto {
        return std::chrono::high_resolution_clock::now();
    };
    using timediff_t = std::chrono::duration<long double>;
    calls_p_second_t count = 0;

    auto start = now();

    long long curr = timediff_t(now() - start).count();
    while (curr < internal) {
        func();
        ++count;
        curr = timediff_t(now() - start).count();
    }

    return count / internal;
}

static bool stopped = false;

void stopWorker() { stopped = true; }

static void worker(moodycamel::BlockingReaderWriterQueue<the_call_t>& toMe,
                   moodycamel::BlockingReaderWriterQueue<bool>& fromMe) {
    while (!stopped) {
        the_call_t call;
        toMe.wait_dequeue(call);
        call();
        if (!fromMe.enqueue(true)) {
            throw std::logic_error("worker 入队错误");
        }
    }
}

int main(int argc, char** argv) {
    std::cout << "什么都不做：" << testFunc([]() {}, 5) << std::endl;

    std::cout << "执行函数调用：" << testFunc([]() { doSomething(); }, 5)
              << std::endl;

    std::cout << "起一个线程执行函数调用："
              << testFunc(
                     []() {
                         std::thread t(doSomething);
                         t.join();
                     },
                     5)
              << std::endl;

    std::cout << "起一个 async 执行函数："
              << testFunc(
                     []() {
                         auto f = std::async(doSomething);
                         f.wait();
                     },
                     5)
              << std::endl;

    moodycamel::BlockingReaderWriterQueue<the_call_t> fromMain;
    moodycamel::BlockingReaderWriterQueue<bool> toMain;
    // std::thread workers{worker, std::ref(fromMain), std::ref(toMain)};
    std::thread workers{[&fromMain, &toMain]() { worker(fromMain, toMain); }};
    std::cout << "反复唤醒同一个线程执行函数调用："
              << testFunc(
                     [&toMain, &fromMain]() {
                         if (!fromMain.enqueue(doSomething)) {
                             throw std::logic_error("testFunc 入队错误");
                         }
                         bool dummy;
                         toMain.wait_dequeue(dummy);
                     },
                     5)
              << std::endl;
    fromMain.enqueue(stopWorker);
    {
        bool dummy;
        toMain.wait_dequeue(dummy);
    }
    workers.join();

    return 0;
}

// 什么都不做：13254190
// 执行函数调用：13203210
// 起一个线程执行函数调用：13814
// 起一个 async 执行函数：12698
// 反复唤醒同一个线程执行函数调用：764973