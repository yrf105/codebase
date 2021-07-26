#include "../EventLoop.h"
#include <sys/timerfd.h>
#include <string.h>
#include <iostream>

tihi::EventLoop* g_loop;

void timeout() {
    // 这里应当 read timerfd，否则事件会一直触发（现在是水平触发模式）
    
    std::cout << "超时" << std::endl;
    g_loop->quit();
}

int main() {

    tihi::EventLoop loop;
    g_loop = &loop;

    int timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    tihi::Channel channel(g_loop, timerfd);
    channel.setReadCallback(timeout);
    channel.enableReading();

    itimerspec howlong;
    bzero(&howlong, sizeof(howlong));
    howlong.it_value.tv_sec = 5;
    timerfd_settime(timerfd, 0, &howlong, nullptr);

    g_loop->loop();

    ::close(timerfd);

    return 0;
}