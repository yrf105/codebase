#ifndef REACTOR_EPOLLER_H
#define REACTOR_EPOLLER_H

#include <boost/noncopyable.hpp>
#include <vector>
#include <chrono>
#include <unordered_map>

struct epoll_event;

namespace tihi {

class EventLoop;
class Channel;

// 1. EPoller 是对 io 多路复用的封装
// 2. EPoller 与 EventLoop 的生命周期相同
// 3. EPoller 不拥有 Channel，即不管理 Channel 的生命周期，Channel 在析构前需自行取消注册
class EPoller : public boost::noncopyable {
public:
    using ChannelList = std::vector<Channel*>;

    EPoller(EventLoop* loop);
    ~EPoller();

    std::chrono::system_clock::time_point  poll(int timeoutMS, ChannelList& activeChannels);

    void updateChannel(Channel* channel);

    EventLoop* ownerLoop() const {
        return ownerLoop_;
    }

    void assertInLoopThread();

private:
    void fillActiveChannels(int nReady, ChannelList& activeChannels) const;

private:
    using ChannelMap = std::unordered_map<int, Channel*>;

    int epollFd_;
    EventLoop* ownerLoop_;
    ChannelMap channels_;
    std::vector<epoll_event> events_;
};

} // namespace tihi

#endif // REACTOR_POLLER_H