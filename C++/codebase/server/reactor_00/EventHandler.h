#ifndef CODEBASE_SERVER_REACTOR_EVENTHANDLER_H
#define CODEBASE_SERVER_REACTOR_EVENTHANDLER_H

#include <sys/epoll.h>

#include <boost/noncopyable.hpp>
#include <memory>

#include "Handle.h"

namespace codebase {

// enum class EventType {
//     ACCEPT_EVENT = 01,
//     READ_EVENT = EPOLLIN,
//     WRITE_EVENT = EPOLLOUT,
//     TIMEOUT_EVENT = 010,
//     SIGNAL_EVENT = 020,
//     CLOSE_EVENT = 040
// };

using EventType = uint32_t;

const EventType READ_EVENT = EPOLLIN;
const EventType WRITE_EVENT = EPOLLOUT;

class EventHandler : public boost::noncopyable {
public:
    using SPtr = std::shared_ptr<EventHandler>;

    virtual int handleAccept() = 0;
    virtual int handleRead() = 0;
    virtual int handleWrite() = 0;
    virtual int handleTimeout() = 0;
    virtual int handleSignal() = 0;
    virtual int handleClose() = 0;
    
    Handle::SPtr getHandle() const;
    EventType eventType() const;

    void setEventType(EventType type);
    void setREventType(EventType type);

private:
    Handle::SPtr handle_;
    uint32_t eventType_;
    uint32_t rEventType_;
};  // EventHandler

}  // namespace codebase

#endif  // CODEBASE_SERVER_REACTOR_EVENTHANDLER_H