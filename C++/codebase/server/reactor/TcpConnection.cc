#include "TcpConnection.h"

#include "EventLoop.h"

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
    (void)n;
    messageCallback_(shared_from_this(), buffer, n);
    // 当 n 为 0 时处理关闭连接
}


}  // namespace tihi