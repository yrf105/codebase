#ifndef REACTOR_LOG_H
#define REACTOR_LOG_H

#include <string.h>

#include <errno.h>
#include <iostream>

#define LOG(message)                                                    \
    do {                                                                \
        std::cout << __FILE__ << ":" << __LINE__ << " " << errno << " " \
                  << strerror(errno) << " " << message << std::endl;    \
    } while (0)

#endif  // REACTOR_LOG_H