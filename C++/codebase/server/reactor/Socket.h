#ifndef REACTOR_SOCKET_H
#define REACTOR_SOCKET_H

#include <boost/noncopyable.hpp>

#include "InetAddress.h"

namespace tihi {

class Socket : public boost::noncopyable {
public:
    explicit Socket(int sockfd);
    Socket(const InetAddress& address);
    ~Socket();

    int fd() const {
        return fd_;
    }

    // 出错则终止
    void bindAddress(const InetAddress& localAddr);
    // 出错则终止
    void listen();
    
    // 正常会返回已连接的文件描述符
    // 出错返回 -1
    int accept(InetAddress& peerAddr);
    
    int connect(InetAddress& peerAddr);

    void setReuseAddr(bool on);

    void shutdownRead();

    void shutdownWrite();

    void setTcpNoDelay(bool on);
    void setTcpKeepAlive(bool on);

private:
    const int fd_;

};

} // namespace tihi

#endif // REACTOR_SOCKET_H