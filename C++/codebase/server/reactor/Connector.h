#ifndef REACTOR_CONNECTOR_H
#define REACTOR_CONNECTOR_H

#include <boost/noncopyable.hpp>
#include <functional>
#include <memory>
#include <atomic>

#include "Socket.h"
#include "Channel.h"
#include "TimerId.h"

namespace tihi {

class EventLoop;
class InetAddress;

class Connector : public boost::noncopyable {
public:
    using NewConnectionCallback = std::function<void(int fd)>;

    Connector(EventLoop* loop, const InetAddress& addr);
    ~Connector();

    void setNewConnectionCallback_(const NewConnectionCallback& cb ) {
        newConnectionCallback_ = cb;
    }

    void start();
    void reStart();
    void stop();

private:
    enum class States {
        kDisconnected,
        kConnecting,
        kConnected,
    };
    
    static const int kMaxRetryDelayMs = 30 * 1000;
    static const int kInitRetryDelayMs = 500;

    void setState(States state) {
        state_ = state;
    }

    void startInLoop();
    void connect();
    void connecting(int sockfd);
    void retry(int sockfd);
    void handleWrite();
    void handleError();

    int removeAndResetChannel();
    void resetChannel();

private:
    EventLoop* loop_;
    InetAddress addr_;
    // 是否发起连接
    std::atomic<bool> connect_;
    std::atomic<States> state_;
    // std::unique_ptr<Socket> socket_;
    std::unique_ptr<Channel> channel_;
    NewConnectionCallback newConnectionCallback_;
    int retryDelayMs_;
    TimerId timerId_;
};

} // namespace tihi

#endif // REACTOR_CONNECTOR_H