#include "SocketsOps.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define SOCK_LOG_ERR_IF(cond, message)                                      \
    do {                                                                    \
        if (cond) {                                                         \
            printf("%s:%d errno: %d strerror: %s %s\n", __FILE__, __LINE__, \
                   errno, strerror(errno), message);                        \
            exit(1);                                                        \
        }                                                                   \
    } while (0)

namespace tihi {
namespace sockets {

int createNonblocking() {
    int sockfd = ::socket(PF_INET, SOCK_CLOEXEC | SOCK_NONBLOCK | SOCK_STREAM,
                          IPPROTO_TCP);
    SOCK_LOG_ERR_IF(sockfd < 0, "socket()");
    return sockfd;
}

void bind(int listenfd, const sockaddr_in* addr) {
    int ret =
        ::bind(listenfd, (sockaddr*)(addr), sizeof(*addr));
    SOCK_LOG_ERR_IF(ret < 0, "bind()");
}

void listen(int listenfd) {
    int ret = ::listen(listenfd, SOMAXCONN);
    SOCK_LOG_ERR_IF(ret < 0, "listen()");
}

int accpet(int listenfd, sockaddr_in* addr) {
    socklen_t addrLen = sizeof(*addr);
    int connfd = ::accept4(listenfd, (sockaddr*)(addr), &addrLen,
                           SOCK_CLOEXEC | SOCK_NONBLOCK);
    return connfd;  // 这里不会直接终止程序
}

void close(int sockfd) {
    int ret = ::close(sockfd);
    SOCK_LOG_ERR_IF(ret < 0, "close()");
}

void toHostPort(char* buf, size_t size, const sockaddr_in& addr) {
    char host[INET_ADDRSTRLEN] = "INVALID";
    const char* ret = inet_ntop(AF_INET, &(addr.sin_addr), host, INET_ADDRSTRLEN);
    SOCK_LOG_ERR_IF(ret == NULL, "inet_ntop");
    uint16_t port = netWorkToHost16(addr.sin_port);
    ::snprintf(buf, size, "%s:%d", host, port);
}

void fromHostPort(const char* ip, uint16_t port, sockaddr_in& addr) {
    addr.sin_family = AF_INET;
    int ret = inet_pton(AF_INET, ip, &addr.sin_addr);
    SOCK_LOG_ERR_IF(ret < 0, "inet_pton");
    addr.sin_port = hostToNetWork16(port);
}

sockaddr_in getLocalAddr(int fd) {
    sockaddr_in localAddr{0};
    socklen_t len = sizeof(localAddr);
    int ret = ::getsockname(fd, (sockaddr*)(&localAddr), &len);
    SOCK_LOG_ERR_IF(ret != 0, "getsockname");
    return localAddr;
}
}  // namespace sockets
}  // namespace tihi