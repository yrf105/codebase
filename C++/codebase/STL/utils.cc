#include "utils.h"

#include "random_number/random_number.h"

#include <sstream>

namespace codebase {

int QueryDatatTotal() {
    int ret = 0;
    std::cout << "输入数据总数<0 ~ " << RAND_MAX << ">: ";
    std::cin >> ret;
    return ret; 
}

int QueryTarget() {
    int ret = 0;
    std::cout << "输入目标数字<0 ~ " << RAND_MAX << ">: ";
    std::cin >> ret;
    return ret; 
}

CodeTimer::CodeTimer() {
}

void CodeTimer::start() {
    ::gettimeofday(&m_start, nullptr);
}

const std::string CodeTimer::stop() {
    struct timeval now;
    ::gettimeofday(&now, nullptr);

    uint64_t s = now.tv_sec - m_start.tv_sec;
    uint64_t ms = (now.tv_usec - m_start.tv_usec) / 1000;

    return std::to_string(s * 1000 + ms) + " ms";
}

CodeTimer::~CodeTimer() {
}

} // namespace codebase