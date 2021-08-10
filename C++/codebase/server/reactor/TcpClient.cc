#include "TcpClient.h"

#include <string>

#include "EventLoop.h"
#include "InetAddress.h"
#include "SocketsOps.h"

namespace tihi {

namespace detail {

    // void removeConnection(EventLoop* loop, const TcpConnection& conn) {
    //     loop->queueInLoop(std::bind(&TcpConnection::connectDestroyed, conn));
    // }

} // namespace detail

TcpClient::TcpClient(EventLoop* loop, const InetAddress& serverAddr)
    : loop_(loop),
      connector_(new Connector(loop, serverAddr)),
      retry_(false),
      connect_(false),
      nextConnId_(1) {
    connector_->setNewConnectionCallback(
        std::bind(&TcpClient::newConnection, this, std::placeholders::_1));
}

TcpClient::~TcpClient() {
    TcpConnection::SPtr conn;
    {
        std::lock_guard<std::mutex> lock(mu_);
        conn = tcpConnection_;
    }
    if (conn) {
        // auto cb = std::bind(&detail::removeConnection, loop_, std::placeholders::_1);
        EventLoop* loop = loop_;
        loop_->runInLoop(std::bind(&TcpConnection::setCloseCallback, conn, [loop](const TcpConnection::SPtr& connArg){
            loop->queueInLoop(std::bind(&TcpConnection::connectDestroyed, connArg));
        }));
    }
}

void TcpClient::connect() {
    connect_ = true;
    connector_->start();
}

void TcpClient::disconnect() {
    connect_ = false;
    if (tcpConnection_ != nullptr) {
        std::lock_guard<std::mutex> lock(mu_);
        tcpConnection_->shutdown();
    }
}

void TcpClient::stop() {
    connect_ = false;
    connector_->stop();
}

void TcpClient::newConnection(int sockfd) {
    loop_->assertInLoopThread();
    InetAddress peerAddr(sockets::getPeerAddr(sockfd));
    std::string name =
        peerAddr.toHostPort() + "#" + std::to_string(nextConnId_);
    ++nextConnId_;

    InetAddress localAddr(sockets::getLocalAddr(sockfd));
    auto conn = std::make_shared<TcpConnection>(loop_, name, sockfd, localAddr,
                                                peerAddr);
    conn->setConnectionCallback(connectionCallback_);
    conn->setMessageCallback(messageCallback_);
    conn->setWriteCompleteCallback(writeCompleteCallback_);
    conn->setCloseCallback(
        std::bind(&TcpClient::removeConnection, this, std::placeholders::_1));
    {
        std::lock_guard<std::mutex> lock(mu_);
        tcpConnection_ = conn;
    }
    conn->connectEstablished();
}

void TcpClient::removeConnection(const TcpConnection::SPtr& conn) {
    loop_->assertInLoopThread();
    assert(loop_ == conn->getLoop());

    {
        std::lock_guard<std::mutex> lock(mu_);
        assert(tcpConnection_ == conn);
        tcpConnection_.reset();
    }

    loop_->queueInLoop(std::bind(&TcpConnection::connectDestroyed, conn));

    if (connect_ && retry_) {
        connector_->reStart();
    }
}

}  // namespace tihi
