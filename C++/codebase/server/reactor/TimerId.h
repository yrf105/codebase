#ifndef REACTOR_TIMERID_H
#define REACTOR_TIMERID_H

namespace tihi {

class Timer;

class TimerId {
public:
    explicit TimerId(Timer* timer) : value_(timer) {}

private:
    Timer* value_;
};

} // namespace tihi

#endif // REACTOR_TIMERID_H