#include <spdlog/spdlog.h>
#include "../EventLoop.h"
#include <iostream>

void threadFunc() {
    tihi::EventLoop loop;
    std::cout << std::this_thread::get_id() << " 开始 " << &loop << " loop\n";
    loop.loop();
    std::cout << &loop << " 结束 loop\n";
}

int main() {

    std::thread t(threadFunc);

    tihi::EventLoop loop;
    std::cout << std::this_thread::get_id() << " 开始 " << &loop << " loop\n";
    loop.loop();
    std::cout << &loop << " 结束 loop\n";

    t.join();

    return 0;
}