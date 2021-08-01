#include "EPoller.h"

#include <assert.h>
#include <sys/epoll.h>

#include <iostream>

#include "Channel.h"
#include "EventLoop.h"
#include "log.h"

namespace tihi {

EPoller::EPoller(EventLoop* loop)
    : epollFd_(::epoll_create1(EPOLL_CLOEXEC)), ownerLoop_(loop), events_(16) {
    assert(epollFd_ >= 0);
}

EPoller::~EPoller() { ::close(epollFd_); }

std::chrono::system_clock::time_point EPoller::poll(
    int timeoutMS, ChannelList& activeChannels) {
    int nReady = epoll_wait(epollFd_, events_.data(),
                            static_cast<int>(events_.size()), timeoutMS);
    if (nReady > 0) {
        if (nReady == static_cast<int>(events_.size())) {
            events_.resize(nReady * 2);
        }

        fillActiveChannels(nReady, activeChannels);
    } else if (nReady == 0) {
        LOG_TRACE << "没有事件发生" << std::endl;
    } else {
        LOG_TRACE << "有错误发生" << std::endl;
    }
    return std::chrono::system_clock::now();
}

void EPoller::updateChannel(Channel* channel) {
    assertInLoopThread();

    int fd = channel->fd();
    epoll_event event;
    event.data.ptr = static_cast<void*>(channel);
    event.events = static_cast<uint32_t>(channel->event());
    int ret = 0;

    if (channel->index() < 0) {
        ret = epoll_ctl(epollFd_, EPOLL_CTL_ADD, fd, &event);
    } else {
        if (channel->isNoneEvent()) {
            ret = epoll_ctl(epollFd_, EPOLL_CTL_DEL, fd, nullptr);
            // 为了在下面将 index 值为 -1，这里先将 fd 值为 -1
            // index 为 -1 表示该 Channel 所负责的 fd 未被 EPoller 管理
            fd = -1;
        } else {
            ret = epoll_ctl(epollFd_, EPOLL_CTL_MOD, fd, &event);
        }
    }
    assert(ret >= 0);
    channel->set_index(fd);
}

void EPoller::assertInLoopThread() { ownerLoop_->assertInLoopThread(); }

void EPoller::fillActiveChannels(int nReady,
                                 ChannelList& activeChannels) const {
    for (int i = 0; i < nReady; ++i) {
        Channel* channel = static_cast<Channel*>(events_[i].data.ptr);
        channel->set_revent(events_[i].events);
        activeChannels.push_back(channel);
    }
}

}  // namespace tihi