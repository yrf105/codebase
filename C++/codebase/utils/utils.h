#ifndef CODEBASE_STL_UTILS_H_
#define CODEBASE_STL_UTILS_H_

#include "time.h"
#include <sys/time.h>

#include <iostream>
#include <string>

namespace codebase {

int QueryDatatTotal();
int QueryTarget();

class CodeTimer {
public:    
    CodeTimer();
    ~CodeTimer();

    void start();
    const std::string stop();

private:
    struct timeval m_start;

};

} // namespace codebase

#endif // CODEBASE_STL_UTILS_H_