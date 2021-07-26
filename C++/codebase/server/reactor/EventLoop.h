#ifndef REACTOR_EVENTLOOP_H
#define REACTOR_EVENTLOOP_H

#include <atomic>
#include <boost/noncopyable.hpp>
#include <thread>
#include <vector>
#include <memory>
#include "EPoller.h"
#include <assert.h>
#include "Channel.h"

namespace tihi {

class Channel;

class EventLoop : public boost::noncopyable { // 不可拷贝
public:
    EventLoop();
    ~EventLoop();

    void loop();

    void updateChannel(Channel* channel) {
        assert(EPoller_->ownerLoop() == this);
        assertInLoopThread();
        EPoller_->updateChannel(channel);
    }

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

    void quit();

private:
    void abortNotInLoopThread();

private:
    using ChannelList = std::vector<Channel*>;

    std::atomic<bool> looping_{false};
    // EventLoop 所在线程，线程只能有一个 EventLoop，EventLoop 只能属于一个线程
    const std::thread::id threadId_;

    std::atomic<bool> quit_{true};
    ChannelList activeChannels_;

    std::unique_ptr<EPoller> EPoller_;
};

}  // namespace tihi

#endif  // REACTOR_EVENTLOOP_H