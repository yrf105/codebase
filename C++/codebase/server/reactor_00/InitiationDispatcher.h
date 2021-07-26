#ifndef CODEBASE_SERVER_REACTOR_INITIATIONDISPATCHER_H
#define CODEBASE_SERVER_REACTOR_INITIATIONDISPATCHER_H

#include <sys/epoll.h>

#include <memory>
#include <unordered_map>
#include <boost/noncopyable.hpp>
#include <mutex>
#include <vector>


#include "EventHandler.h"
#include "Handle.h"
#include "SynchronousEventDemultiplexer.h"

namespace codebase {

class InitiationDispatcher : public boost::noncopyable {

public:

    void handleEvents();
    void registerHandler(EventHandler::SPtr handler);
    void removeHandler(EventHandler::SPtr handler);

private:

private:
    int listenFd_;
    std::mutex mu_;
    std::unordered_map<int, EventHandler::SPtr> handlers_;
    SynchronousEventDemultiplexer::UPtr eventDemultiplexer;
};

} // namespace codebase

#endif // CODEBASE_SERVER_REACTOR_INITIATIONDISPATCHER_H