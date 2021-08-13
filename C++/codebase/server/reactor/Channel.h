#ifndef REACTOR_CHANNEL_H
#define REACTOR_CHANNEL_H

#include "boost/noncopyable.hpp"

#include <functional>
#include <chrono>

namespace tihi {

class EventLoop;

// 1. 一个 Channel 只属于一个 EventLoop，一个 EventLoop 可以有多个 Channel
// 2. 一个 Channel 负责一个 fd 的事件分发，但是不拥有该 fd，即不负责管理 fd 的生命周期
// 3. Channel 采用基于对象的思路，用户通过向 Channel 注册可调用对象实现多态，而不继承 Channel
// 4. Channel 的生命周期由拥有它的类负责，Channel 通常被其他类直接或间接拥有
class Channel : public boost::noncopyable {
public:
    using EventCallback = std::function<void()>;
    using ReadEventCallback = std::function<void(std::chrono::system_clock::time_point)>;

    Channel(EventLoop* loop, int fd);
    ~Channel();

    // 由 EventLoop 调用，负责根据 revent_ 调用 errorCallback_ 或 readCallback_ 或 writeCallback_
    void handleEvent(std::chrono::system_clock::time_point receiveTimepoint);

    void setErrorCallback(const EventCallback& callback) {
        errorCallback_ = callback;
    }
    void setReadCallback(const ReadEventCallback& callback) {
        readCallback_ = callback;
    }
    void setWriteCallback(const EventCallback& callback) {
        writeCallback_ = callback;
    }
    void setCloseCallback(const EventCallback& callback) {
        closeCallback_ = callback;
    }

    void enableReading() {
        event_ |= kReadEvent;
        update();
    }
    void enableWriting() {
        event_ |= kWriteEvent;
        update();
    }
    void disableWriting() {
        event_ &= ~kWriteEvent;
        update();
    }
    void disableAll() {
        if (event_ == kNoneEvent) {
            return ;
        }
        event_ = kNoneEvent;
        update();
    }

    bool isWriting() const {
        return event_ & kWriteEvent;
    }

    int fd() const {
        return fd_;
    }
    int event() const {
        return event_;
    }
    void set_revent(int revt) {
        revent_ = revt;
    }
    bool isNoneEvent() const {
        return event_ == kNoneEvent;
    }
    EventLoop* ownerLoop() const {
        return loop_;
    }

    int index() const {
        return idx_;
    }
    void set_index(int idx) {
        idx_ = idx;
    }

private:
    // update 会调用 loop_ 的 updateChannel，loop_ 的 updateChannel 会调用 poller 的 updateChannel
    void update();

private:
    static const int kNoneEvent;
    static const int kReadEvent;
    static const int kWriteEvent;

    EventLoop* loop_;
    const int fd_;
    // 用户关注的事件
    int event_;
    // poller 返回的事件
    int revent_;

    // 供 Poller 来标识该 Channel 负责的 fd 的状态，-1 表示未在管理，>= 0 表示正在管理
    int idx_; // for Poller

    EventCallback errorCallback_;
    ReadEventCallback readCallback_;
    EventCallback writeCallback_;
    EventCallback closeCallback_;

    bool eventHandling_;
};

} // namespace tihi

#endif // REACTOR_CHANNEL_H
