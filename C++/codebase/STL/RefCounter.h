#ifndef CODEBASE_STL_REFCOUNTER_H
#define CODEBASE_STL_REFCOUNTER_H

#include <stdint.h>

namespace codebase {

template <typename T> class smart_ptr;

class RefCounter {

template <typename T> 
friend class smart_ptr;

private:
    RefCounter();
    RefCounter(const RefCounter& rhs);
    RefCounter& operator=(RefCounter rhs);
    ~RefCounter();

    void try_decrease();
    // void try_increase();
    void swap(RefCounter& rhs);
    void swap(RefCounter& lhs, RefCounter& rhs);

private:
    int64_t* count_;
};

} // namespcae codebase

#endif // CODEBASE_STL_REFCOUNTER_H