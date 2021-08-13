#ifndef REACTOR_TCPCLIENT_H
#define REACTOR_TCPCLIENT_H

#include <atomic>
#include "TcpConnection.h"
#include "Connector.h"
#include <boost/noncopyable.hpp>
#include <mutex>

namespace tihi {

class EventLoop;
class InetAddress;

// 一个 TcpClient 只管理一个 TcpConnection
class TcpClient : public boost::noncopyable {
public:
    TcpClient(EventLoop* loop, const InetAddress& serverAddr);
    ~TcpClient();

    void connect();
    void disconnect();
    void stop();

    TcpConnection::SPtr tcpConnection() const {
        std::lock_guard<std::mutex> lock(mu_);
        return tcpConnection_;
    }

    bool retry() const {
        return retry_;
    }
    void enableRetry() {
        retry_ = true;
    }

    void setConnectionCallback(const TcpConnection::ConnectionCallback& cb) {
        connectionCallback_ = cb;
    }
    void setMessageCallback(const TcpConnection::MessageCallback& cb) {
        messageCallback_ = cb;
    }
    void setWriteCompleteCallback(const TcpConnection::WriteCompleteCallback& cb) {
        writeCompleteCallback_ = cb;
    }

private:
    void newConnection(int sockfd);
    void removeConnection(const TcpConnection::SPtr& conn);

private:
    EventLoop* loop_;
    Connector::SPtr connector_;
    
    TcpConnection::ConnectionCallback connectionCallback_;
    TcpConnection::MessageCallback messageCallback_;
    TcpConnection::WriteCompleteCallback writeCompleteCallback_;

    std::atomic<bool> retry_;
    std::atomic<bool> connect_;

    int nextConnId_;
    mutable std::mutex mu_;
    TcpConnection::SPtr tcpConnection_;
};

} // namespace tihi

#endif // REACTOR_TCPCLIENT_H
