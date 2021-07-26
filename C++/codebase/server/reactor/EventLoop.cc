#include "EventLoop.h"

#include <assert.h>
#include <sys/epoll.h>
#include <sys/poll.h>

#include <iostream>

#include "stdlib.h"

namespace tihi {

thread_local EventLoop* t_loopInThisThread{nullptr};

EventLoop* getEventLoopOfCurrentThread() { return t_loopInThisThread; }

EventLoop::EventLoop()
    : looping_(false),
      threadId_(
          std::this_thread::get_id()) {  // 将当前线程的 id 保存在 threadId_ 里
    // 判断当前线程是否已经存在一个 EventLoop 了
    if (t_loopInThisThread) {
        // 若存在则终止程序
        std::cout << "当前线程已经存在一个 EventLoop 了" << std::endl;
        ::abort();
    } else {
        // 若不存在，则指定自己作为当前线程的 EventLoop
        t_loopInThisThread = this;
    }
}

// EventLoop 不必是堆对象，生命周期通常与所在线程相同
EventLoop::~EventLoop() {
    assert(!looping_);
    t_loopInThisThread = nullptr;
}

void EventLoop::loop() {
    assert(!looping_);
    assertInLoopThread();
    looping_ = true;

    // 什么都不做，等 5 秒就退出

    // epoll 不能这么用，无法达到阻塞 5s 的效果，必须注册 fd 进去才行
    // int epollFd = ::epoll_create1(EPOLL_CLOEXEC);
    // ::epoll_wait(epollFd, nullptr, 0, 5 * 1000);

    ::poll(nullptr, 0, 3 * 1000);

    std::cout << "EventLoop " << static_cast<void*>(this) << " 停止 loop\n";
    looping_ = false;
}

void EventLoop::abortNotInLoopThread() {
    std::cout << "该 EventLoop 应当在线程 " << threadId_
              << " 中运行，当前线程是 " << std::this_thread::get_id()
              << std::endl;
    ::abort();
}

}  // namespace tihi