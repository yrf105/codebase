#include "Acceptor.h"

#include "EventLoop.h"
#include "Channel.h"
#include "SocketsOps.h"

namespace tihi {

Acceptor::Acceptor(EventLoop* loop, const InetAddress& listenAddr)
    : loop_(loop),
      acceptSocket_(sockets::createNonblocking()),
      acceptChannel_(loop_, acceptSocket_.fd()),
      listening_(false),
      newConnectionCallback_() {
    acceptSocket_.setReuseAddr(true);
    acceptSocket_.bindAddress(listenAddr);
    acceptChannel_.setReadCallback(std::bind(&Acceptor::handleRead, this));
}

void Acceptor::listen() {
    loop_->assertInLoopThread();
    listening_ = true;
    acceptSocket_.listen();
    acceptChannel_.enableReading();
}

void Acceptor::handleRead() {
    InetAddress peerAddr{0};
    int connfd = acceptSocket_.accept(peerAddr);
    if (connfd >= 0) {
        if (newConnectionCallback_) {
            newConnectionCallback_(connfd, peerAddr);
        } else {
            sockets::close(connfd);
        }
    }
}

}  // namespace tihi