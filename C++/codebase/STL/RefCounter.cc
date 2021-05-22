#include "RefCounter.h"

#include <iostream>
#include <utility>

namespace codebase {

RefCounter::RefCounter() : count_{new int64_t(1)} {}

RefCounter::RefCounter(const RefCounter& rhs) : count_(rhs.count_){
    ++(*count_);
}

RefCounter& RefCounter::operator=(RefCounter rhs) {
    try_decrease();
    swap(rhs);
    ++(*count_);
    return *this;
}

RefCounter::~RefCounter() {
    std::cout << "RefCounter::~RefCounter()\n";
    try_decrease();
}

void RefCounter::try_decrease() {
    if (count_) {
        --(*count_);
        std::cout << "RefCounter::try_decrease()\n" << "*count_ " << *count_ << std::endl;
        if (*count_ <= 0) {
            delete count_;
            count_ = nullptr;
        }
    }
}

void RefCounter::swap(RefCounter& rhs) {
    std::swap(count_, rhs.count_);
}

void RefCounter::swap(RefCounter& lhs, RefCounter& rhs) {
    using std::swap;
    swap(lhs.count_, rhs.count_);
}


} // namespace codebase