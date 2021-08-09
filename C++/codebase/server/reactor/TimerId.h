#ifndef REACTOR_TIMERID_H
#define REACTOR_TIMERID_H

namespace tihi {

class Timer;
class TimerQueue;

class TimerId {
    friend TimerQueue;
public:
    explicit TimerId(Timer* timer, uint64_t seq) : value_(timer), sequence_(seq) {}

private:
    // 这里的 Timer 可以是 std::weak_ptr 
    Timer* value_;
    uint64_t sequence_;
};

} // namespace tihi

#endif // REACTOR_TIMERID_H