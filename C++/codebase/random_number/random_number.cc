#include "random_number.h"

namespace codebase {

int rand(bool is_srand) {
    if (is_srand) {
        srand((unsigned)time(nullptr));
    }
    return ::rand();
}

} // namespace tihi