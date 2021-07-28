#include "Timer.h"

namespace tihi {

void Timer::restart(TimePoint now) {
    if (repeat_) {
        expiration_ = now + interval_;
    } else {
        expiration_ = std::chrono::system_clock::time_point{};
    }
}


} // namespace tihi