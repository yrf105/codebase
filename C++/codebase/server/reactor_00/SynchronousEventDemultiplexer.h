#ifndef CODEBASE_SERVER_REACTOR_SYNCHRONOUSEVENTDEMULTIPLEXER_H
#define CODEBASE_SERVER_REACTOR_SYNCHRONOUSEVENTDEMULTIPLEXER_H

#include <sys/epoll.h>
#include <sys/types.h>

#include <boost/noncopyable.hpp>
#include <memory>
#include <vector>
#include <unordered_map>

#include "EventHandler.h"

namespace codebase {

class SynchronousEventDemultiplexer : public boost::noncopyable {
public:
    using UPtr = std::unique_ptr<SynchronousEventDemultiplexer>;
    using EventHandlerList = std::vector<EventHandler::SPtr>; 

    SynchronousEventDemultiplexer();
    ~SynchronousEventDemultiplexer();

    void registerHandler(EventHandler::SPtr handler);
    void removeHandler(EventHandler::SPtr handler);

    void select(EventHandlerList& activeEventHandlers);

private:
    void update(EventHandler::SPtr handler, int op);

private:
    int epollFd_;
    std::vector<epoll_event> events_;
    std::unordered_map<int, EventHandler::SPtr> handlers_;
};

}  // namespace codebase

#endif  // CODEBASE_SERVER_REACTOR_SYNCHRONOUSEVENTDEMULTIPLEXER_H