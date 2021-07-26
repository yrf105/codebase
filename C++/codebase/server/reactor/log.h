#ifndef REACTOR_LOG_H
#define REACTOR_LOG_H

#include <errno.h>
#include <string.h>

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <string>

#define LOG(message)                                                    \
    do {                                                                \
        std::cout << __FILE__ << ":" << __LINE__ << " " << errno << " " \
                  << strerror(errno) << " " << message << std::endl;    \
    } while (0)

#endif  // REACTOR_LOG_H

static std::string nowStr() {
    auto now =
        std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::stringstream ss;
    ss << std::put_time(std::localtime(&now), "%F %T");
    return ss.str();
}

static std::string shortFileName(const std::string& filename) {
    int n = filename.size() - 1;
    while (n > 0) {
        if (filename[n] == '/') {
            break;
        }
        --n;
    }
    return filename.substr(n + 1);
}

enum class LOGLEVEL { TRACE = 1, INFO = 2, WARN = 3, FATAL = 4 };

const LOGLEVEL CURLEVEL = LOGLEVEL::TRACE;

#define LOG_TRACE                                                             \
    if (CURLEVEL <= LOGLEVEL::TRACE)                                          \
    std::cout << nowStr() << " " << shortFileName(__FILE__) << ":" << __LINE__ << " " \
              << errno << " " << strerror(errno) << " "
