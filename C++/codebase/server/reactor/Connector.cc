#include "Connector.h"

#include <sys/socket.h>
#include <sys/types.h> /* See NOTES */

#include "EventLoop.h"
#include "SocketsOps.h"
#include "log.h"

namespace tihi {

const int Connector::kMaxRetryDelayMs;

Connector::Connector(EventLoop* loop, const InetAddress& addr)
    : loop_(loop),
      addr_(addr),
      connect_(false),
      state_(States::kDisconnected),
      retryDelayMs_(kInitRetryDelayMs),
      timerId_(nullptr, 0) {}

Connector::~Connector() {
    loop_->cancelTimer(timerId_);
    assert(!channel_);
}

void Connector::start() {
    connect_ = true;
    loop_->runInLoop(std::bind(&Connector::startInLoop, this));
}

void Connector::startInLoop() {
    loop_->assertInLoopThread();
    assert(state_ == States::kDisconnected);
    // 该函数在 loop_ 的任务队列里等待执行时，
    // 用户可能执行 stop()，停止了 Connector
    // 所以要判断一下是否还要进行连接
    if (connect_) {
        connect();
    } else {
        LOG_TRACE << "Connector 已被 stop" << std::endl;
    }
}

void Connector::connect() {
    int sockfd = sockets::createNonblocking();
    int ret = sockets::connect(sockfd, &(addr_.getSockAddrInet()));
    int savedErrno = ret == 0 ? 0 : errno;
    switch (savedErrno) {
        case 0:
        case EINPROGRESS:
        case EINTR:
        case EISCONN: {
            connecting(sockfd);
            break;
        }
        case EAGAIN:
        case EADDRINUSE:
        case EADDRNOTAVAIL:
        case ECONNREFUSED:
        case ENETUNREACH: {
            retry(sockfd);
            break;
        }
        case EACCES:
        case EPERM:
        case EAFNOSUPPORT:
        case EALREADY:
        case EBADF:
        case EFAULT:
        case ENOTSOCK: {
            LOG_TRACE << "connect error " << savedErrno << std::endl;
            sockets::close(sockfd);
            break;
        }

        default: {
            LOG_TRACE << "未知错误 " << savedErrno << std::endl;
            sockets::close(sockfd);
            // connectErrorCallback_();
            break;
        }
    }
}

void Connector::connecting(int sockfd) {
    setState(States::kConnecting);
    assert(!channel_);
    channel_.reset(new Channel(loop_, sockfd));
    channel_->setWriteCallback(std::bind(&Connector::handleWrite, this));
    channel_->setErrorCallback(std::bind(&Connector::handleError, this));
    // 连接建立会触发可写事件
    channel_->enableWriting();
}

void Connector::retry(int sockfd) {
    sockets::close(sockfd);
    setState(States::kDisconnected);
    if (connect_) {
        LOG_TRACE << "重新尝试连接" << std::endl;
        loop_->runAfter(std::chrono::milliseconds(retryDelayMs_),
                        std::bind(&Connector::startInLoop, this));
        retryDelayMs_ = std::min(retryDelayMs_ * 2, kMaxRetryDelayMs);
    } else {
        LOG_TRACE << "Connector 已被 stop" << std::endl;
    }
}

void Connector::reStart() {
    loop_->assertInLoopThread();
    setState(States::kDisconnected);
    connect_ = true;
    retryDelayMs_ = kInitRetryDelayMs;
    startInLoop();
}

void Connector::stop() {
    connect_ = false;
    loop_->cancelTimer(timerId_);
}

void Connector::handleWrite() {
    LOG_TRACE << "Connector::handleWrite" << std::endl;
    if (state_ == States::kConnecting) {
        int sockfd = removeAndResetChannel();
        int err = sockets::getSocketError(sockfd);
        if (err) {
            LOG_TRACE << "Connector::handleWrite sock_err " << err << std::endl;
            retry(sockfd);
        } else if (sockets::isSelfConnection(sockfd)) {
            LOG_TRACE << "Connector::handleWrite 自连接" << std::endl;
        } else {
            setState(States::kConnected);
            if (connect_) {
                newConnectionCallback_(sockfd);
            } else {
                sockets::close(sockfd);
            }
        }
    } else {
        assert(state_ == States::kDisconnected);
    }
}

int Connector::removeAndResetChannel() {
    channel_->disableAll();
    loop_->updateChannel(channel_.get());
    int sockfd = channel_->fd();
    // 不能直接 reset channel 因为现在在 channel 的回调函数里
    loop_->queueInLoop(std::bind(&Connector::resetChannel, this));
    return sockfd;
}

void Connector::resetChannel() { channel_.reset(); }


void Connector::Connector::handleError() {
    LOG_TRACE << "Connector::handleError" << std::endl;
    assert(state_ == States::kConnecting);

    int sockfd = removeAndResetChannel();
    int err = sockets::getSocketError(sockfd);
    LOG_TRACE << "Connector::handleError sock_err " << err << std::endl;
    retry(sockfd);
}

}  // namespace tihi