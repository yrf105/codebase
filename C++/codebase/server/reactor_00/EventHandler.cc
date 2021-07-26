#include "EventHandler.h"

namespace codebase {

Handle::SPtr EventHandler::getHandle() const {
    return handle_;
}

EventType EventHandler::eventType() const {
    return eventType_;
}

} // namespace codebase