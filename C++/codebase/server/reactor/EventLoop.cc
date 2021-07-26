#include "EventLoop.h"

#include <sys/epoll.h>
#include <sys/poll.h>

#include "log.h"
#include <memory>

#include "stdlib.h"

namespace tihi {

thread_local EventLoop* t_loopInThisThread{nullptr};

EventLoop* getEventLoopOfCurrentThread() { return t_loopInThisThread; }

EventLoop::EventLoop()
    : looping_(false),
      threadId_(
          std::this_thread::get_id()),  // 将当前线程的 id 保存在 threadId_ 里
      quit_(true),
      EPoller_(new EPoller(
          this)) {  // one loop per thread 带来的好处，同一个 EventLoop
                    // 里的代码一定在同一个线程里执行，不需要加锁，再构造函数里可以把
                    // this 指针暴露出去（在多线程编程里是要避免的）
    // 判断当前线程是否已经存在一个 EventLoop 了
    if (t_loopInThisThread) {
        // 若存在则终止程序
        LOG("当前线程已经存在一个 EventLoop 了");
        ::abort();
    } else {
        // 若不存在，则指定自己作为当前线程的 EventLoop
        t_loopInThisThread = this;
    }
}

// EventLoop 不必是堆对象，生命周期通常与所在线程相同
EventLoop::~EventLoop() {
    assert(!looping_);
    assert(quit_);
    t_loopInThisThread = nullptr;
}

void EventLoop::loop() {
    assert(!looping_);
    assertInLoopThread();
    looping_ = true;
    quit_ = false;

    while (!quit_) {
        activeChannels_.clear();
        auto timeout = EPoller_->poll(1 * 1000, activeChannels_);
        LOG(timeout.time_since_epoch().count());
        for (auto activechannel : activeChannels_) {
            activechannel->handleEvent();
        }
    }

    LOG("EventLoop 停止了 loop");
    looping_ = false;
}

void EventLoop::abortNotInLoopThread() {
    LOG("该 EventLoop 不属于该线程");
    ::abort();
}

void EventLoop::quit() { quit_ = true; }


}  // namespace tihi