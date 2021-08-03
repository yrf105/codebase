#ifndef REACTOR_CONNECTION_H
#define REACTOR_CONNECTION_H

#include <functional>
#include <memory>

#include "Channel.h"
#include "InetAddress.h"
#include "Socket.h"
#include "Timer.h"
#include "Buffer.h"

namespace tihi {

class EventLoop;
class Buffer;

// 1. TcpConnection 表示一次连接，一旦断开就没啥用了
// 2. TcpConnection 没有发起连接的功能，它接受一个已连接的 fd
// 构造自己，并在自己析构时关闭该 fd
class TcpConnection : public boost::noncopyable, public std::enable_shared_from_this<TcpConnection> {
public:
    using SPtr = std::shared_ptr<TcpConnection>;
    using ConnectionCallback = std::function<void(const SPtr&)>;
    using MessageCallback =
        std::function<void(const SPtr&, Buffer*, Timer::TimePoint)>;
    using CloseCallback = std::function<void(const SPtr&)>;

    TcpConnection(EventLoop* loop, const std::string& name, int connfd, 
                  const InetAddress& localAddr, const InetAddress& peerAddr);
    ~TcpConnection();

    EventLoop* getLoop() const {
        return loop_;
    }
    const std::string& name() const {
        return name_;
    }
    const InetAddress& localAddr() const {
        return localAddr_;
    }
    const InetAddress& peerAddr() const {
        return peerAddr_;
    }
    bool connected() const {
        return state_ == StateE::kConnected;
    }

    // 连接和断开都会调用 ConnectionCallback_
    void setConnectionCallback(const ConnectionCallback& cb) {
        connectionCallback_ = cb;
    }
    void setMessageCallback(const MessageCallback& cb) {
        messageCallback_ = cb;
    }
    void setCloseCallback(const CloseCallback& cb) {
        closeCallback_ = cb;
    }

    // 仅在连接建立时调用 1 次
    void connectEstablished();
    // 当 TcpServer 从它的 connections_ 中移除连接是调用该函数
    // connectDestroyed 是 TcpConnection 析构前最后调用的函数
    void connectDestroyed();

    void send(const std::string& message);
    void shutdown();

private:
    enum class StateE {
        kConnecting,
        kConnected,
        kDisconnecting,
        kDisconnected,
    };

private:
    void setState(StateE state) { state_ = state; };
    void handleRead(std::chrono::system_clock::time_point receiveTimepoint);
    void handleError();
    void handleWrite();
    void handleClose();

    void sendInloop(const std::string& message);
    void shutdownInloop();

private:
    EventLoop* loop_;
    const std::string name_;
    StateE state_;
    std::unique_ptr<Socket> socket_;
    std::unique_ptr<Channel> channel_;
    InetAddress localAddr_;
    InetAddress peerAddr_;
    ConnectionCallback connectionCallback_;
    MessageCallback messageCallback_;
    CloseCallback closeCallback_;
    Buffer inputBuffer_;
    Buffer outputBuffer_;
};

}  // namespace tihi

#endif  // REACTOR_CONNECTION_H