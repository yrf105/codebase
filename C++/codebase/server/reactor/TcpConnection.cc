#include "TcpConnection.h"

#include "EventLoop.h"
#include "SocketsOps.h"
#include "log.h"

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
    channel_->setReadCallback(
        std::bind(&TcpConnection::handleRead, this, std::placeholders::_1));
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

void TcpConnection::handleRead(
    std::chrono::system_clock::time_point receiveTimepoint) {
    int savedErrno = 0;
    ssize_t n = inputBuffer_.readFd(socket_->fd(), &savedErrno);
    if (n > 0) {
        messageCallback_(shared_from_this(), &inputBuffer_, receiveTimepoint);
    } else if (n == 0) {
        handleClose();
    } else if (n < 0) {
        errno = savedErrno;
        LOG_TRACE << "TcpConnection::handleRead error" << std::endl;
        handleError();
    }
}

void TcpConnection::connectDestroyed() {
    loop_->assertInLoopThread();
    assert(state_ == StateE::kConnected || state_ == StateE::kDisconnecting);
    setState(StateE::kDisconnected);
    channel_->disableAll();  // 可能不经过 handleClose 直接调用 connectDestroyed
    connectionCallback_(shared_from_this());

    loop_->updateChannel(channel_.get());
}


void TcpConnection::handleError() {
    int err = sockets::getSocketError(socket_->fd());
    LOG_TRACE << name_ << " 连接出错 " << err << std::endl;
}

void TcpConnection::handleWrite() {
    loop_->assertInLoopThread();
    if (channel_->isWriting()) {  // 若注册了写事件则继续将缓冲区中的数据发送完
        ssize_t n = ::write(channel_->fd(), outputBuffer_.peek(),
                            outputBuffer_.readableBytes());
        if (n >= 0) {
            outputBuffer_.retrieve(n);
            if (outputBuffer_.readableBytes() == 0) {
                channel_->disableWriting();
                if (writeCompleteCallback_) {
                    loop_->queueInLoop(std::bind(writeCompleteCallback_, shared_from_this()));
                }
                if (state_ == StateE::kDisconnecting) {
                    shutdownInloop();
                }
            } else {
                LOG_TRACE << "数据还没有发送完" << std::endl;
            }
        } else {
            LOG_TRACE << "TcpConnection::handleWrite 写入错误" << std::endl;
        }
    } else {
        LOG_TRACE << "没有注册可写事件" << std::endl;
    }
}

// 被 TcpServer::removeConnection 调用
void TcpConnection::handleClose() {
    loop_->assertInLoopThread();
    assert(state_ == StateE::kConnected || state_ == StateE::kDisconnecting);
    channel_->disableAll();
    closeCallback_(shared_from_this());
}

void TcpConnection::send(const std::string& message) {
    if (state_ == StateE::kConnected) {
        if (loop_->isInLoopThread()) {
            sendInloop(message);
        } else {
            loop_->runInLoop(
                std::bind(&TcpConnection::sendInloop, this, message));
        }
    }
}

void TcpConnection::sendInloop(const std::string& message) {
    loop_->assertInLoopThread();
    ssize_t nwrote = 0;
    // 若发送缓冲中没有数据则尝试直接发送
    if (!channel_->isWriting() && outputBuffer_.readableBytes() == 0) {
        nwrote = ::write(channel_->fd(), message.data(), message.size());
        if (nwrote >= 0) {
            if (static_cast<size_t>(nwrote) < message.size()) {
                LOG_TRACE << "没有发完，还有更多的数据需要发送" << std::endl;
            } else if (writeCompleteCallback_) {
                // writeCompleteCallback_(shared_from_this());
                loop_->queueInLoop(std::bind(writeCompleteCallback_, shared_from_this()));
            }
        } else {
            nwrote = 0;
            if (errno != EWOULDBLOCK) {
                LOG_TRACE << "TcpConnection::sendInloop error" << std::endl;
            }
        }
    }

    assert(nwrote >= 0);
    // 若数据没有发送完毕，则将其放入发送缓冲，并注册可写事件
    if (static_cast<size_t>(nwrote) < message.size()) {
        outputBuffer_.append(message.data() + nwrote, message.size() - nwrote);
        if (!channel_->isWriting()) {
            channel_->enableWriting();
        }
    }
}

void TcpConnection::shutdown() {
    if (state_ == StateE::kConnected) {
        setState(StateE::kDisconnecting);
        loop_->runInLoop(std::bind(&TcpConnection::shutdownInloop, this));
    }
}

void TcpConnection::shutdownInloop() {
    loop_->assertInLoopThread();
    if (!channel_->isWriting()) {
        socket_->shutdownWrite();
    }
}

void TcpConnection::setTcpNoDelay(bool on) {
    socket_->setTcpNoDelay(on);
}

void TcpConnection::setTcpKeepAlive(bool on) {
    socket_->setTcpKeepAlive(on);
}

// 利用全局对象初始化在程序开始之前就忽略 SIGPIPIE
IgnoreSigPipe __ignoreSigPipeObj;

}  // namespace tihi