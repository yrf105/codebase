#ifndef REACTOR_EVENTLOOP_H
#define REACTOR_EVENTLOOP_H

#include <assert.h>

#include <atomic>
#include <boost/noncopyable.hpp>
#include <memory>
#include <thread>
#include <vector>
#include <mutex>

#include "Channel.h"
#include "EPoller.h"
#include "TimerQueue.h"
#include "Callbacks.h"
#include "TimerId.h"

namespace tihi {

class Channel;

class EventLoop : public boost::noncopyable {  // 不可拷贝
public:
    using Functor = std::function<void()>;

    EventLoop();
    ~EventLoop();

    void loop();

    // 在 EventLoop 所在 IO 线程中监听 channel 负责 fd 上的给定事件，
    // 并在事件触发时回调 channel 中给定事件对应的回调函数
    void updateChannel(Channel* channel) {
        assert(EPoller_->ownerLoop() == this);
        assertInLoopThread();
        EPoller_->updateChannel(channel);
    }
    
    // 在 EventLoop 所在 IO 线程中设置定时器，
    // 定时器到期时执行指定的回调函数 callback
    TimerId runAt(const Timer::TimePoint& time, const TimerCallback& callback);
    TimerId runAfter(const std::chrono::nanoseconds delay, const TimerCallback& callback);
    TimerId runEvery(const std::chrono::nanoseconds interval, const TimerCallback& callback);

    // 1. 在当前 IO 线程的中执行 cb 用户回调
    // 2. 当其他线程希望向指定线程添加定时任务时，
    //    可以通过该函数将添加定时器的任务转移到指定 IO 线程中进行，
    //    进而避免在添加定时器时加锁
    // 3. 若执行 runInLoop 的线程就是 loop 所在 IO 线程，则立即执行 cb
    void runInLoop(const Functor& cb);

    // 1. 功能基本与 runInLoop 相同，不同之处在于就算在指定 IO 线程中添加 cb 也不会马上执行
    void queueInLoop(const Functor& cb);

    // 该函数负责向 eventFd 写入数据，以触发写事件，进而唤醒 loop
    void wakeup();

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

    // 当需要执行用户回调时需要立即唤醒 loop，
    // 唤醒 loop 的任务通过触发 eventFd 上的读事件来实现

    // 该函数负责处理 eventFd 上的读事件
    void handleRead();
    void doPendingFunctors();
    

private:
    using ChannelList = std::vector<Channel*>;

    std::atomic<bool> looping_{false};
    // EventLoop 所在线程，线程只能有一个 EventLoop，EventLoop 只能属于一个线程
    const std::thread::id threadId_;

    std::atomic<bool> quit_{true};
    ChannelList activeChannels_;

    std::unique_ptr<EPoller> EPoller_;

    std::unique_ptr<TimerQueue> timerQueue_;

    std::atomic<bool> callingPendingFunctors_;
    int wakeupFd_;
    std::unique_ptr<Channel> wakeupChannel_;
    std::mutex mu_;
    std::vector<Functor> pendingFunctors_;
};

}  // namespace tihi

#endif  // REACTOR_EVENTLOOP_H