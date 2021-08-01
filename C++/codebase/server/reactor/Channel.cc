#include "Channel.h"

#include <sys/epoll.h>

#include <iostream>

#include "EventLoop.h"
#include "log.h"

namespace tihi {

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = EPOLLIN;
const int Channel::kWriteEvent = EPOLLOUT;

Channel::Channel(EventLoop* loop, int fd)
    : loop_(loop),
      fd_(fd),
      event_(kNoneEvent),
      revent_(kNoneEvent),
      idx_(-1),
      eventHandling_(false) {}

Channel::~Channel() {
    // 断言当正在处理该channel 上的事件时，该 channel 不会析构
    assert(!eventHandling_);
}


void Channel::handleEvent() {
    eventHandling_ = true;

    // EPOLLHUP 表示读写都关闭，或读到文件结尾会收到 EPOLLHUP 事件
    if ((revent_ & EPOLLHUP) && !(revent_ & EPOLLIN)) {
        if (closeCallback_) {
            closeCallback_();
        }
    }
    // EPOLLPRI 表示接收到紧急数据（带外数据）
    // EPOLLRDHUP 表示读关闭 https://blog.csdn.net/zhouguoqionghai/article/details/94591475
    if (revent_ & (EPOLLIN | EPOLLRDHUP | EPOLLPRI)) {
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

    eventHandling_ = false;
}

void Channel::update() { loop_->updateChannel(this); }

}  // namespace tihi
