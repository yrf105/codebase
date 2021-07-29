#include "../EventLoop.h"
#include <iostream>
#include "../EventLoopThread.h"

static void Test_runInLoop() {
    tihi::EventLoop loop;

    loop.runInLoop([&loop]{
        std::cout << "test runInLoop()" << std::endl;
        loop.runAfter(std::chrono::seconds(2), [&loop]{
            std::cout << "runAfter" << std::endl;
            loop.queueInLoop([&loop]{
                std::cout << "queueInLoop" << std::endl;
                loop.quit();
            });
        });
    });

    loop.loop();
}

static void Test_threadSafeAddTimer() {
    tihi::EventLoop loop;
    std::thread t1([&loop]{
        std::cout << "在其他线程中向 IO 线程添加定时器" << std::endl;
        loop.runAfter(std::chrono::seconds(1), [&loop]{
            loop.quit();
        });
    });
    loop.loop();
    t1.join();
}

static void Test_EventLoopThread() {
    std::cout << "main " << std::this_thread::get_id() << std::endl;
    tihi::EventLoopThread eLoopThread;
    tihi::EventLoop* loop = eLoopThread.startLoop();
    loop->runInLoop([]{
        std::cout << "EventLoopThread " << std::this_thread::get_id() << std::endl;
    });
    std::this_thread::sleep_for(std::chrono::seconds(1));
    loop->runAfter(std::chrono::seconds(2), []{
        std::cout << "EventLoopThread " << std::this_thread::get_id() << std::endl;
    });
    std::this_thread::sleep_for(std::chrono::seconds(3));
    loop->quit();
}

int main(int argc, char** argv) {

    // Test_runInLoop();
    // Test_threadSafeAddTimer();
    Test_EventLoopThread();

    return 0;
}