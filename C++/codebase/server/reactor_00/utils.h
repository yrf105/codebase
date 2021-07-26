#ifndef CODEBASE_SERVER_REACTOR_UTILS_H
#define CODEBASE_SERVER_REACTOR_UTILS_H

#include <errno.h>
#include <string.h>

#define LOG_ERR_IF(cond, message)                                           \
    do {                                                                    \
        if (cond) {                                                         \
            printf("%s:%d errno: %d strerror: %s %s\n", __FILE__, __LINE__, \
                   errno, strerror(errno), message);                        \
        }                                                                   \
    } while (0)

namespace codebase {}  // namespace codebase

#endif