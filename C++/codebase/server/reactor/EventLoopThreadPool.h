#ifndef REACTOR_EVENTLOOPTHREADPOOL_H
#define REACTOR_EVENTLOOPTHREADPOOL_H

#include <boost/noncopyable.hpp>
#include <memory>
#include <thread>
#include <vector>
#include "EventLoopThread.h"

namespace tihi {
class EventLoop;
class EventLoopThreadPool : public boost::noncopyable {
public:
    EventLoopThreadPool(EventLoop* baseLoop);
    ~EventLoopThreadPool();
    void setThreadNum(int numThreads) {
        numThreads_ = numThreads;
    }
    void start();
    EventLoop* getNextLoop();

private:
    EventLoop* baseLoop_;
    
    bool started_;
    int numThreads_;
    int next_;
    std::vector<std::unique_ptr<EventLoopThread>> threads_;
    std::vector<EventLoop*> loops_;
};

}  // namespace tihi

#endif  // REACTOR_EVENTLOOPTHREADPOOL_H