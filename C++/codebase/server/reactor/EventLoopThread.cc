#include "EventLoopThread.h"

#include <functional>

#include "EventLoop.h"

namespace tihi {

EventLoopThread::EventLoopThread() : exiting_(false), loop_(nullptr), mu_(), cond_(),
    threadPtr_() {}

EventLoopThread::~EventLoopThread() {
    exiting_ = true;
    loop_->quit();
    threadPtr_->join();
}

EventLoop* EventLoopThread::startLoop() {
    threadPtr_ = std::make_unique<std::thread>(std::bind(&EventLoopThread::threadFunc, this));

    {
        std::unique_lock<std::mutex> lock(mu_);
        cond_.wait(lock, [this]{ return loop_ != nullptr; });
    }

    return loop_;
}


void EventLoopThread::threadFunc() {
    EventLoop loop;
    {
        std::lock_guard<std::mutex> lock(mu_);
        loop_ = &loop;
        cond_.notify_one();
    }
    loop.loop();
}

}  // namespace tihi