#include "../EventLoop.h"

tihi::EventLoop* loop;

void threadFunc() {
    loop->loop();
}

int main() {
    tihi::EventLoop eventLoop;
    loop = &eventLoop;

    std::thread t1(threadFunc);

    loop->loop();

    t1.join();
}