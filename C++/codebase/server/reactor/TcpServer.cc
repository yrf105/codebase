#include "TcpServer.h"

#include "SocketsOps.h"
#include "EventLoop.h"
#include "log.h"

namespace tihi {

TcpServer::TcpServer(EventLoop* loop, const InetAddress& listenAddr, const std::string& name)
    : loop_(loop),
      name_(name == "" ? listenAddr.toHostPort() : name),
      acceptorUPtr_(new Acceptor(loop, listenAddr)),
      connectionCallback_(),
      messageCallback_(),
      started_(false),
      nextConnId_(0),
      connections_() {
    acceptorUPtr_->setNewConnectionCallback(
        std::bind(&TcpServer::newConnection, this, std::placeholders::_1,
                  std::placeholders::_2));
}

TcpServer::~TcpServer() {}

void TcpServer::start() {
    if (!started_) {
        started_ = true;
    }
    if (!acceptorUPtr_->listening()) {
        loop_->runInLoop(std::bind(&Acceptor::listen, acceptorUPtr_.get()));
    }
}

void TcpServer::newConnection(int connfd, const InetAddress& peerAddr) {
    loop_->assertInLoopThread();
    std::string connName = peerAddr.toHostPort() + std::to_string(nextConnId_);
    ++nextConnId_;

    LOG_TRACE << connName << " 连接" << std::endl;
    InetAddress localAddr(sockets::getLocalAddr(connfd));
    // auto conn = std::make_shared<TcpConnection>(loop_, connName, connfd, localAddr, peerAddr);
    TcpConnection::SPtr conn = TcpConnection::SPtr(new TcpConnection(loop_, connName, connfd, localAddr, peerAddr));
    connections_[connName] = conn;
    conn->setConnectionCallback(connectionCallback_);
    conn->setMessageCallback(messageCallback_);
    conn->connectEstablished();
}

}  // namespace tihi