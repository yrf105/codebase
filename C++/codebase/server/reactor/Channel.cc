#include "Channel.h"

#include <sys/epoll.h>
#include <iostream>

#include "EventLoop.h"

namespace tihi {

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = EPOLLIN;
const int Channel::kWriteEvent = EPOLLOUT;

Channel::Channel(EventLoop* loop, int fd)
    : loop_(loop), fd_(fd), event(kNoneEvent), revent_(kNoneEvent), idx_(-1) {}

void Channel::handleEvent() {
    // EPOLLPRI 表示接收到紧急数据（带外数据）
    if (revent_ & (EPOLLIN | EPOLLHUP | EPOLLPRI)) {
        if (readCallback_) {
            readCallback_();
        }
    }
    if (revent_ & (EPOLLERR)) {
        if (errorCallback_) {
            errorCallback_();
        }
    }
    if (revent_ & (EPOLLOUT)) {
        if (writeCallback_) {
            writeCallback_();
        }
    }
}

void Channel::update() { loop_->updateChannel(this); }

}  // namespace tihi
