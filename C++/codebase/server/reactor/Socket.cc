#include "Socket.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/tcp.h> // TCP_NODELAY
#include <assert.h>
#include <string.h>

#include "SocketsOps.h"

namespace tihi {

Socket::Socket(int sockfd) : fd_(sockfd) {}

Socket::~Socket() {
    sockets::close(fd_);
}

void Socket::bindAddress(const InetAddress& localAddr) {
    sockets::bind(fd_, &localAddr.getSockAddrInet());
}

void Socket::listen() {
    sockets::listen(fd_);
}

int Socket::accept(InetAddress& peerAddr) {
    sockaddr_in addr{0};
    int connfd = sockets::accpet(fd_, &addr);
    if (connfd >= 0) {
        peerAddr.setSockAddrInet(addr);
    }
    return connfd;
}

void Socket::setReuseAddr(bool on) {
    int opVal = on ? 1 : 0;
    int ret = ::setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &opVal, sizeof(opVal));
    assert(ret == 0);
}

void Socket::shutdownWrite() {
    int ret = ::shutdown(fd_, SHUT_WR);
    assert(ret == 0);
}

void Socket::shutdownRead() {
    int ret = ::shutdown(fd_, SHUT_RD);
    assert(ret == 0);
}

void Socket::setTcpNoDelay(bool on) {
    int optVal = on ? 1 : 0;
    ::setsockopt(fd_, IPPROTO_TCP, TCP_NODELAY, &optVal, sizeof(optVal));
}

void Socket::setTcpKeepAlive(bool on) {
    int optVal = on ? 1 : 0;
    ::setsockopt(fd_, SOL_SOCKET, SO_KEEPALIVE, &optVal, sizeof(optVal));
}


}  // namespace tihi