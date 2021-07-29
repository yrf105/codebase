#ifndef REACTOR_EVENTLOOPTHREAD_H
#define REACTOR_EVENTLOOPTHREAD_H

#include <boost/noncopyable.hpp>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <memory>

namespace tihi {

class EventLoop;

class EventLoopThread : public boost::noncopyable {
public:
    EventLoopThread();
    ~EventLoopThread();

    EventLoop* startLoop();

private:
    void threadFunc();

private:
    bool exiting_;
    EventLoop* loop_;
    std::mutex mu_;
    std::condition_variable cond_;
    std::unique_ptr<std::thread> threadPtr_;
};

} // namespace tihi

#endif // REACTOR_EVENTLOOPTHREAD_H