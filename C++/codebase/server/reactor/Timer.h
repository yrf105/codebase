#ifndef REACTOR_TIMER_H
#define REACTOR_TIMER_H

#include <boost/noncopyable.hpp>
#include <chrono>

#include "Callbacks.h"

namespace tihi {

class Timer : public boost::noncopyable {
public:
    using TimePoint = std::chrono::system_clock::time_point;

    // 需要指定回调函数、超时时间点、循环定时器的周期
    Timer(const TimerCallback& cb, TimePoint when,
          std::chrono::nanoseconds interval)
        : callback_(cb), expiration_(when), interval_(interval) , repeat_(interval != std::chrono::nanoseconds{}) {}

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

public:
    static const uint64_t kNanoSecondsPerSecond = 1000000000;

private:
    const TimerCallback callback_;
    // 定时器超时的时间点
    TimePoint expiration_;
    // 循环定时器的循环周期
    const std::chrono::nanoseconds interval_;
    // 该定时器是否是循环定时器
    const bool repeat_;
};

}  // namespace tihi

#endif  // REACTOR_TIMER_H