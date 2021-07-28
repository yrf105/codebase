#include "../EventLoop.h"
#include <iostream>

tihi::EventLoop* g_loop;

void funcRunEvery() {
    std::cout << "runEvery 定时器到期\n";
}

void funcRunAfter() {
    std::cout << "runAfter 定时器到期\n";
}

void funcRunAt() {
    std::cout << "runAt 定时器超时\n";
    g_loop->quit();
}

int main() {

    tihi::EventLoop loop;
    g_loop = &loop;

    g_loop->runAt(std::chrono::system_clock::now() + std::chrono::seconds(10), funcRunAt);
    g_loop->runAfter(std::chrono::seconds(5), funcRunAfter);
    g_loop->runEvery(std::chrono::seconds(1), funcRunEvery);

    g_loop->loop();

    std::cout << "main 退出\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));

    return 0;
}