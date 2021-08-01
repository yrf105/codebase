#include "TcpConnection.h"

#include "EventLoop.h"
#include "log.h"
#include "SocketsOps.h"

namespace tihi {

TcpConnection::TcpConnection(EventLoop* loop, const std::string& name,
                             int connfd, const InetAddress& localAddr,
                             const InetAddress& peerAddr)
    : loop_(loop),
      name_(name),
      state_(StateE::kConnecting),
      socket_(new Socket(connfd)),
      channel_(new Channel(loop, connfd)),
      localAddr_(localAddr),
      peerAddr_(peerAddr) {
    channel_->setReadCallback(std::bind(&TcpConnection::handleRead, this));
    channel_->setErrorCallback(std::bind(&TcpConnection::handleError, this));
    channel_->setCloseCallback(std::bind(&TcpConnection::closeCallback_, this));
    channel_->setWriteCallback(std::bind(&TcpConnection::handleWrite, this));
}

TcpConnection::~TcpConnection() {}

void TcpConnection::connectEstablished() {
    loop_->assertInLoopThread();
    assert(state_ != StateE::kConnected);
    setState(StateE::kConnected);
    channel_->enableReading();

    connectionCallback_(shared_from_this());
}

void TcpConnection::handleRead() {
    char buffer[65536];
    ssize_t n = ::read(socket_->fd(), buffer, sizeof buffer);
    if (n > 0) {
        messageCallback_(shared_from_this(), buffer, n);
    } else if (n == 0) {
        handleClose();
    } else if (n < 0) {
        handleError();
    }
}

void TcpConnection::connectDestroyed() {
    loop_->assertInLoopThread();
    assert(state_ == StateE::kConnected);
    setState(StateE::kDisconnected);
    channel_->disableAll(); // 可能不经过 handleClose 直接调用 connectDestroyed
    connectionCallback_(shared_from_this());

    loop_->updateChannel(channel_.get());
}


void TcpConnection::handleError() {
    int err = sockets::getSocketError(socket_->fd());
    LOG_TRACE << name_ << " 连接出错 " << err << std::endl;
}

void TcpConnection::handleWrite() {}

// 被 TcpServer::removeConnection 调用
void TcpConnection::handleClose() {
    loop_->assertInLoopThread();
    assert(state_ == StateE::kConnected);
    channel_->disableAll();
    closeCallback_(shared_from_this());
}

}  // namespace tihi