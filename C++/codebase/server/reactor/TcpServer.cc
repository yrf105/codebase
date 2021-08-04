#include "TcpServer.h"

#include "EventLoop.h"
#include "SocketsOps.h"
#include "log.h"

namespace tihi {

TcpServer::TcpServer(EventLoop* loop, const InetAddress& listenAddr,
                     const std::string& name)
    : loop_(loop),
      name_(name == "" ? listenAddr.toHostPort() : name),
      acceptorUPtr_(new Acceptor(loop, listenAddr)),
      connectionCallback_(),
      messageCallback_(),
      started_(false),
      nextConnId_(0),
      connections_(),
      pool_(new EventLoopThreadPool(loop)) {
    acceptorUPtr_->setNewConnectionCallback(
        std::bind(&TcpServer::newConnection, this, std::placeholders::_1,
                  std::placeholders::_2));
}

TcpServer::~TcpServer() {}

void TcpServer::start() {
    if (!started_) {
        started_ = true;
        pool_->start();
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
    // auto conn = std::make_shared<TcpConnection>(loop_, connName, connfd,
    // localAddr, peerAddr);
    EventLoop* ioLoop = pool_->getNextLoop();
    TcpConnection::SPtr conn = TcpConnection::SPtr(
        new TcpConnection(ioLoop, connName, connfd, localAddr, peerAddr));
    connections_[connName] = conn;
    conn->setConnectionCallback(connectionCallback_);
    conn->setMessageCallback(messageCallback_);
    conn->setCloseCallback(
        std::bind(&TcpServer::removeConnection, this, std::placeholders::_1));
    conn->setWriteCompleteCallback(writeCompleteCallback_);
    ioLoop->runInLoop(std::bind(&TcpConnection::connectEstablished, conn));
}

void TcpServer::removeConnection(const TcpConnection::SPtr& connection) {
    loop_->runInLoop(std::bind(&TcpServer::removeConnectionInLoop, this, connection));
}

void TcpServer::removeConnectionInLoop(const TcpConnection::SPtr& connection) {
    loop_->assertInLoopThread();
    size_t n = connections_.erase(connection->name());
    assert(n == 1);
    (void)n;
    // 这里不能直接这样调用 connectDestroyed()，
    // 因为若用户不持有 connection，那么 connection 的引用计数在经过上面 erase 后会降为 1，
    // 可能在执行 connectDestroyed 之前被析构
    // connection->connectDestroyed();

    EventLoop* ioLoop = connection->getLoop();
    // 使用 bind 增加 connection 的引用计数
    ioLoop->queueInLoop(std::bind(&TcpConnection::connectDestroyed, connection));
}

void TcpServer::setThreadNum(int numThreads) {
    assert(numThreads >= 0);
    pool_->setThreadNum(numThreads);
}


}  // namespace tihi