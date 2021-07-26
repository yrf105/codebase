#include "SynchronousEventDemultiplexer.h"
#include "utils.h"
#include <sys/unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include "InitiationDispatcher.h"
#include <assert.h>

namespace codebase {

SynchronousEventDemultiplexer::SynchronousEventDemultiplexer() : epollFd_(epoll_create1(EPOLL_CLOEXEC)), events_(16) {
    LOG_ERR_IF(epollFd_ < 0, "epoll_create1 error");
}

SynchronousEventDemultiplexer::~SynchronousEventDemultiplexer() {
    close(epollFd_);
}

void SynchronousEventDemultiplexer::registerHandler(EventHandler::SPtr handler) {

    if (handler == nullptr) {
        return ;
    }

    int fd = handler->getHandle()->fd();
    epoll_event event{0};
    event.data.ptr = handler.get();
    event.events = handler->eventType();
    int ret = 0;
    if (handlers_.count(fd) && handlers_[fd]->eventType() != handler->eventType()) {
        ret = epoll_ctl(epollFd_, EPOLL_CTL_MOD, fd, &event);
    } else {
        ret = epoll_ctl(epollFd_, EPOLL_CTL_ADD, fd, &event);
    }

    LOG_ERR_IF(ret, "epoll_ctl error");
    handlers_[fd] = handler;
}

void SynchronousEventDemultiplexer::removeHandler(EventHandler::SPtr handler) {
    if (handler == nullptr) {
        return ;
    }

    int ret;
    int fd = handler->getHandle()->fd();
    if (handlers_.count(fd)) {
        handlers_.erase(fd);
        ret = epoll_ctl(epollFd_, EPOLL_CTL_DEL, fd, nullptr);
        LOG_ERR_IF(ret, "epoll_ctl EPOLL_CTL_DEL error");
    }
}

void SynchronousEventDemultiplexer::update(EventHandler::SPtr handler, int op) {
    
}


void SynchronousEventDemultiplexer::select(EventHandlerList& activeEventHandlers) {
    while (true) {
        int nReady = epoll_wait(epollFd_, events_.data(), static_cast<int>(events_.size()), -1);
        
        if (nReady < 0) {
            if (errno == EINTR) {
                continue ;
            }
            LOG_ERR_IF(nReady < 0, "epoll_wait error");
        }

        if (nReady == 0) {
            continue ;
        }

        if (nReady == static_cast<int>(events_.size())) {
            events_.resize(nReady * 2);
        }

        for (int i = 0; i < nReady; ++i) {
            
            // static_cast<EventHandler*>(events_[i].data.ptr)->setREventType(static_cast<EventType>(events_[i].events));
            // activeEventHandlers.push_back(events_[i].data.ptr);
        }
    }
}


} // namespace codebase