#ifndef REACTOR_TIMERQUEUE_H
#define REACTOR_TIMERQUEUE_H

#include <boost/noncopyable.hpp>
#include "Timer.h"
#include <memory>
#include <set>
#include <vector>
#include "Channel.h"

namespace tihi {

class EventLoop;
class TimerId;

// 1. TimerQueue 负责组织未到期的 Timer，能够根据当前时间快速返回已到期的 Timer
// 2. 可以快速的添加和删除 Timer
// 3. 将 pair<TimePoint, Timer*> 作为 key，并使用 set 管理
// 4. 成员函数只能在所属的 loop 里调用，因此不必加锁
// 5. 使用一个 channel 观察 timerFd 上的可读事件
// 6. 不保证会按时回调
class TimerQueue : public boost::noncopyable {
public:
    TimerQueue(EventLoop* loop);
    ~TimerQueue();

    // 所需参数与 Timer 构造函数所需参数相同
    TimerId addTimer(const TimerCallback& cb_,
                     Timer::TimePoint when,
                     std::chrono::nanoseconds interval);
    // 取消指定的定时器
    void cancel(TimerId timerId);

private:
    using Entry = std::pair<Timer::TimePoint, Timer*>;
    using TimerList = std::set<Entry>;
    using ActiveTimer = std::pair<Timer*, uint64_t>;
    using ActiveTimerSet = std::set<ActiveTimer>;

    // 处理 timerfd 上的可读事件
    void handleRead();
    // 获得所有到期的定时器
    std::vector<Entry> getExpired(Timer::TimePoint now);
    
    // 对 expired 中需要重复的 timer 重新进行设置
    void reset(std::vector<Entry>& expired, Timer::TimePoint now);

    bool insert(Timer* timer);
    // void insert(std::unique_ptr<Timer>& timer);

    void addTimerInLoop(Timer* timer);

    void cancelInLoop(TimerId timerId);

private:
    EventLoop* loop_;
    int timerFd_;
    Channel timerFdChannel_;
    // 根据到期时间排序的计时器列表
    TimerList timers_;

    // for cancel()
    std::atomic<bool> callingExpiredTimers_;
    // TimerId 不负责其所代表的 Timer 的生命周期，
    // 要通过 TimerId 删除某个定时器，
    // 就要现在 activeTimers_ 中找到它
    ActiveTimerSet activeTimers_;
    // 自删除的 timer 会被添加到 cancelingTimers_ 中
    ActiveTimerSet cancelingTimers_;
};

} // namespace tihi

#endif // REACTOR_TIMERQUEUE_H