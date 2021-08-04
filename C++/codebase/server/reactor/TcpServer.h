#ifndef REACTOR_TCPSERVER_H
#define REACTOR_TCPSERVER_H

#include <boost/noncopyable.hpp>
#include <unordered_map>
#include "Acceptor.h"
#include "TcpConnection.h"
#include "Callbacks.h"
#include <memory>
#include <string>
#include "InetAddress.h"

namespace tihi {

class EventLoop;

class TcpServer : public boost::noncopyable {
public:
    TcpServer(EventLoop* loop, const InetAddress& listenAddr, const std::string& name = "");
    ~TcpServer();

    void start();

    void setConnectionCallback(const TcpConnection::ConnectionCallback& cb) {
        connectionCallback_ = cb;
    }
    void setMessageCallback(const TcpConnection::MessageCallback& cb) {
        messageCallback_ = cb;
    }
    // 设置低水位回调函数，当发送缓冲区为空时会调用所指定的回调函数
    void setWriteCompleteCallback(const TcpConnection::WriteCompleteCallback& cb) {
        writeCompleteCallback_ = cb;
    }

private:
    void newConnection(int connfd, const InetAddress& peerAddr);
    void removeConnection(const TcpConnection::SPtr& connection);

private:
    using ConnectionMap = std::unordered_map<std::string, TcpConnection::SPtr>;

    EventLoop* loop_;
    std::string name_;
    std::unique_ptr<Acceptor> acceptorUPtr_;
    TcpConnection::ConnectionCallback connectionCallback_;
    TcpConnection::MessageCallback messageCallback_;
    TcpConnection::WriteCompleteCallback writeCompleteCallback_;
    bool started_;
    int nextConnId_;
    ConnectionMap connections_;
};

} // namespace tihi

#endif // REACTOR_TCPSERVER_H