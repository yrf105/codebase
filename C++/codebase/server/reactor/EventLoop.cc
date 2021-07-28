#include "EventLoop.h"

#include <sys/epoll.h>
#include <sys/poll.h>

#include <memory>

#include "log.h"
#include "stdlib.h"

namespace tihi {

thread_local EventLoop* t_loopInThisThread{nullptr};

EventLoop* getEventLoopOfCurrentThread() { return t_loopInThisThread; }

EventLoop::EventLoop()
    : looping_(false),
      threadId_(
          std::this_thread::get_id()),  // 将当前线程的 id 保存在 threadId_ 里
      quit_(true),
      EPoller_(new EPoller(this)),
      timerQueue_(new TimerQueue(this)) {
    // one loop per thread 带来的好处，同一个 EventLoop
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
        LOG_TRACE << timeout.time_since_epoch().count() << " " << activeChannels_.size() << std::endl;
        for (auto activechannel : activeChannels_) {
            activechannel->handleEvent();
        }
    }

    LOG_TRACE << "EventLoop 停止了 loop" << std::endl;
    looping_ = false;
}

TimerId EventLoop::runAt(const Timer::TimePoint& time, const TimerCallback& callback) {
    return timerQueue_->addTimer(callback, time, std::chrono::nanoseconds{});
}

TimerId EventLoop::runAfter(const std::chrono::nanoseconds delay, const TimerCallback& callback) {
    return timerQueue_->addTimer(callback, std::chrono::system_clock::now() + delay, std::chrono::nanoseconds{});
}

TimerId EventLoop::runEvery(const std::chrono::nanoseconds interval, const TimerCallback& callback) {
    return timerQueue_->addTimer(callback, std::chrono::system_clock::now() + interval, interval);
}

void EventLoop::abortNotInLoopThread() {
    LOG("该 EventLoop 不属于该线程");
    ::abort();
}

void EventLoop::quit() {
    quit_ = true;
    // 这里应当唤醒 poll，若不唤醒，则必须等待程序从 poll 中返回并检查 quit_
    // 时才会退出
}


}  // namespace tihi