#include <iostream>

// class Widget {
// public:
//     static void* operator new(size_t size, std::ostream& os) {
//         // ...
//     }

//     static void operator delete(void* pMemory, std::ostream& os) {
//         // ...
//     }

//     static void operator delete(void* pMemory) {
//         // ...
//     }
// };

class StandardNewDeleteForms {
public:
    static void* operator new(std::size_t size) {
        return ::operator new(size);
    }
    static void operator delete(void* pMemoty) {
        return ::operator delete(pMemoty);
    }

    static void* operator new(std::size_t size, void* ptr) {
        return ::operator new(size, ptr);
    }
    static void operator delete(void* pMemoty, void* ptr) {
        return ::operator delete(pMemoty, ptr);
    }

    static void* operator new(std::size_t size, const std::nothrow_t& nt) {
        return ::operator new(size, nt);
    }
    static void operator delete(void* pMemoty, const std::nothrow_t& nt) {
        return ::operator delete(pMemoty, nt);
    }
};

class Widget : public StandardNewDeleteForms {
public:
    using StandardNewDeleteForms::operator new;
    using StandardNewDeleteForms::operator delete;

    static void* operator new(size_t size, std::ostream& os);
    static void operator delete(void* pMemory, std::ostream& os);
    static void operator delete(void* pMemory);
};

int main() {}