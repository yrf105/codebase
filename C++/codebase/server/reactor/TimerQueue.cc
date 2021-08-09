#include "TimerQueue.h"

#include <assert.h>
#include <sys/timerfd.h>

#include "EventLoop.h"
#include "TimerId.h"
#include "log.h"

namespace tihi {

namespace detail {

int createTimerFd() {
    int fd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    if (fd < 0) {
        LOG_TRACE << "创建 timerfd 失败" << std::endl;
    }
    return fd;
}

timespec howMuchTimeFromNow(Timer::TimePoint when) {
    int64_t nanoseconds = (when.time_since_epoch() -
                           std::chrono::system_clock::now().time_since_epoch())
                              .count();
    if (nanoseconds < 100000) {
        nanoseconds = 100000;
    }
    timespec tp{0};
    tp.tv_sec = static_cast<time_t>(nanoseconds / Timer::kNanoSecondsPerSecond);
    tp.tv_nsec = static_cast<long>(nanoseconds % Timer::kNanoSecondsPerSecond);
    return tp;
}

void resetTimerFd(int timerFd, Timer::TimePoint expiration) {
    itimerspec newValue{0};
    itimerspec oldValue{0};

    newValue.it_value = howMuchTimeFromNow(expiration);
    int ret = ::timerfd_settime(timerFd, 0, &newValue, &oldValue);
    if (ret < 0) {
        LOG_TRACE << "timer_settime error" << std::endl;
    }
}

void readTimerFd(int timerFd, Timer::TimePoint now) {
    uint64_t howmany;
    ssize_t n = ::read(timerFd, &howmany, sizeof(howmany));
    LOG_TRACE << "TimerQueue::handleTimer() " << n << " at "
              << now.time_since_epoch().count() << std::endl;
    if (n != sizeof(howmany)) {
        LOG_TRACE << "readTimerFd 读了 " << n << " 字节" << std::endl;
    }
}

}  // namespace detail

TimerQueue::TimerQueue(EventLoop* loop)
    : loop_(loop),
      timerFd_(detail::createTimerFd()),
      timerFdChannel_(loop, timerFd_),
      timers_() {
    timerFdChannel_.setReadCallback(std::bind(&TimerQueue::handleRead, this));
    timerFdChannel_.enableReading();
}

TimerQueue::~TimerQueue() {
    ::close(timerFd_);
    for (auto& it : timers_) {
        delete it.second;
    }
}

TimerId TimerQueue::addTimer(const TimerCallback& cb_, Timer::TimePoint when,
                             std::chrono::nanoseconds interval) {
    Timer* timer = new Timer(cb_, when, interval);
    // 将添加计时器的操作转移到目标 IO 线程中，实现线程安全
    loop_->runInLoop(std::bind(&TimerQueue::addTimerInLoop, this, timer));
    return TimerId(timer, timer->sequence());
}

void TimerQueue::addTimerInLoop(Timer* timer) {
    // 添加定时器只在 IO 线程里进行
    loop_->assertInLoopThread();
    bool earliestChanged = insert(timer);

    if (earliestChanged) {
        // 下一个超时的就是当前的计时器，直接把它设置进去
        detail::resetTimerFd(timerFd_, timer->expiration());
    }
}


void TimerQueue::cancel(TimerId timerId) {
    loop_->runInLoop(std::bind(&TimerQueue::cancelInLoop, this, timerId));
}

void TimerQueue::handleRead() {
    loop_->assertInLoopThread();
    Timer::TimePoint now(std::chrono::system_clock::now());
    detail::readTimerFd(timerFd_, now);

    std::vector<Entry> expired = getExpired(now);

    callingExpiredTimers_ = true;
    cancelingTimers_.clear();

    for (const auto& entry : expired) {
        entry.second->run();
    }

    callingExpiredTimers_ = false;

    reset(expired, now);
}

std::vector<TimerQueue::Entry> TimerQueue::getExpired(Timer::TimePoint now) {
    std::vector<Entry> expired;
    auto sentry = std::make_pair(now, reinterpret_cast<Timer*>(UINTPTR_MAX));
    auto it = timers_.lower_bound(sentry);
    assert(it == timers_.end() || now < it->first);

    // https://zh.cppreference.com/w/cpp/container/unordered_set/extract
    // extract 是从 set 带走仅移动对象的唯一方式：
    // for (auto e = timers_.begin(); e != it;) {
    //     expired.emplace_back(std::move(timers_.extract(e++).value()));
    // }

    // std::move(timers_.begin(), it, std::back_inserter(expired));
    // expired.insert(expired.end(), std::make_move_iterator(timers_.begin()),
    // std::make_move_iterator(it)); expired.insert(expired.end(),
    // timers_.begin(), it); std::copy(timers_.begin(), it,
    // std::back_inserter(expired));

    std::copy(timers_.begin(), it, std::back_inserter(expired));

    timers_.erase(timers_.begin(), it);
    return expired;
}

void TimerQueue::reset(std::vector<Entry>& expired, Timer::TimePoint now) {
    for (auto& entry : expired) {
        ActiveTimer activeTimer(entry.second, entry.second->sequence());
        if (entry.second->repeat() &&
            cancelingTimers_.find(activeTimer) == cancelingTimers_.end()) {
            entry.second->restart(now);
            insert(entry.second);
        } else {
            delete entry.second;
        }
    }

    Timer::TimePoint nextExpiration;
    if (!timers_.empty()) {
        nextExpiration = timers_.begin()->first;
    }

    if (nextExpiration != std::chrono::system_clock::time_point{}) {
        detail::resetTimerFd(timerFd_, nextExpiration);
    }
}

// void TimerQueue::insert(std::unique_ptr<Timer>& timer) {
//     std::pair<TimerList::iterator, bool> result =
//         timers_.insert(std::make_pair(timer->expiration(), std::move(timer)));
//     assert(result.second);
// }


bool TimerQueue::insert(Timer* timer) {
    // 标识当前要插入的定时器是否是最早超时的定时器
    bool earliestChanged = false;

    auto when = timer->expiration();
    auto it = timers_.begin();

    // 定时器列表为空，或者当前定时器的比定时器列表里最早超时的定时器的超时时间还要早
    if (it == timers_.end() || when < it->first) {
        earliestChanged = true;
    }

    std::pair<TimerList::iterator, bool> result =
        timers_.insert(std::make_pair(when, timer));
    assert(result.second);

    return earliestChanged;
}

void TimerQueue::cancelInLoop(TimerId timerId) {
    loop_->assertInLoopThread();
    assert(timers_.size() == activeTimers_.size());

    ActiveTimer timer(timerId.value_, timerId.sequence_);
    auto it = activeTimers_.find(timer);

    if (it != activeTimers_.end()) {
        size_t n = timers_.erase(Entry(it->first->expiration(), it->first));
        assert(n == 1);
        (void)n;
        delete it->first;
        activeTimers_.erase(it);
    } else if (callingExpiredTimers_) {
        // 处理自删除的情况，即在定时器的回调函数中执行删除当前定时器的情况
        cancelingTimers_.insert(timer);
    }
    assert(timers_.size() == activeTimers_.size());
}

}  // namespace tihi