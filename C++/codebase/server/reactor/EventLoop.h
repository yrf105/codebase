#ifndef REACTOR_EVENTLOOP_H
#define REACTOR_EVENTLOOP_H

#include <atomic>
#include <boost/noncopyable.hpp>
#include <thread>

namespace tihi {

class EventLoop : public boost::noncopyable {  // 不可拷贝
public:
    EventLoop();
    ~EventLoop();

    void loop();

    // 断言 EventLoop 属于当前线程
    void assertInLoopThread() {
        if (!isInLoopThread()) {
            abortNotInLoopThread();
        }
    }

    bool isInLoopThread() const {
        return std::this_thread::get_id() == threadId_;
    }

    // 返回当前线程拥有的 EventLoop 对象，可能为空
    static EventLoop* getEventLoopOfCurrentThread();

private:
    void abortNotInLoopThread();

private:
    std::atomic<bool> looping_{false};
    // EventLoop 所在线程，线程只能有一个 EventLoop，EventLoop 只能属于一个线程
    const std::thread::id threadId_;
};

}  // namespace tihi

#endif  // REACTOR_EVENTLOOP_H