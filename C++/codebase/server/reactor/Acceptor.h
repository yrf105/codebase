#ifndef REACTOR_ACCEPTOR_H
#define REACTOR_ACCEPTOR_H

#include <boost/noncopyable.hpp>
#include <functional>
#include "Socket.h"
#include "InetAddress.h"
#include "Channel.h"

namespace tihi {

class EventLoop;

// Acceptor 不单独出现，而是作为 TcpServer 的组件，TcpServer 负责 Acceptor 的生命周期
class Acceptor : public boost::noncopyable {
public:
    using NewConnectionCallback = std::function<void(int connfd, const InetAddress& address)>;

    Acceptor(EventLoop* loop, const InetAddress& listenAddr);
    
    void setNewConnectionCallback(const NewConnectionCallback& cb) {
        newConnectionCallback_ = cb;
    }

    bool listening() const {
        return listening_;
    }

    void listen();

private:
    void handleRead();

private:
    EventLoop* loop_;
    Socket acceptSocket_;
    Channel acceptChannel_;
    bool listening_;
    NewConnectionCallback newConnectionCallback_;
};

} // namespace tihi

#endif // REACTOR_ACCEPTOR_H