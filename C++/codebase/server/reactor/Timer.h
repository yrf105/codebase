#ifndef REACTOR_TIMER_H
#define REACTOR_TIMER_H

#include <boost/noncopyable.hpp>
#include <chrono>
#include <atomic>

#include "Callbacks.h"

namespace tihi {

class Timer : public boost::noncopyable {
public:
    using TimePoint = std::chrono::system_clock::time_point;

    // 需要指定回调函数、超时时间点、循环定时器的周期
    Timer(const TimerCallback& cb, TimePoint when,
          std::chrono::nanoseconds interval)
        : callback_(cb), expiration_(when), interval_(interval) , repeat_(interval != std::chrono::nanoseconds{}), sequence_(++s_numCreated_) {}

    void run() const {
        callback_();
    }

    TimePoint expiration() const {
        return expiration_;
    }

    bool repeat() const {
        return repeat_;
    }

    void restart(TimePoint now);

    uint64_t sequence() const {
        return sequence_;
    }

public:
    static const uint64_t kNanoSecondsPerSecond = 1000000000;

private:
    static std::atomic<uint64_t> s_numCreated_;

private:
    const TimerCallback callback_;
    // 定时器超时的时间点
    TimePoint expiration_;
    // 循环定时器的循环周期
    const std::chrono::nanoseconds interval_;
    // 该定时器是否是循环定时器
    const bool repeat_;
    // 唯一标识一个定时器（不能把地址作为定时器的唯一标识，先后创建的不同定时器的地址可能相同）
    const uint64_t sequence_;
};

}  // namespace tihi

#endif  // REACTOR_TIMER_H