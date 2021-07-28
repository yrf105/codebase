#ifndef REACTOR_CALLBACKS_H
#define REACTOR_CALLBACKS_H

#include <functional>

namespace tihi {

using TimerCallback = std::function<void()>;

} // namespace tihi

#endif // REACTOR_CALLBACKS_H