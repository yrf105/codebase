#include "Timer.h"

namespace tihi {

std::atomic<uint64_t> Timer::s_numCreated_ = {0};

void Timer::restart(TimePoint now) {
    if (repeat_) {
        expiration_ = now + interval_;
    } else {
        expiration_ = std::chrono::system_clock::time_point{};
    }
}

} // namespace tihi