#ifndef CODEBASE_STL_STRING_H_
#define CODEBASE_STL_STRING_H_

#include <utility>
#include <string.h>
#include <iostream>

namespace codebase {

class String {
friend std::ostream& operator<< (std::ostream& os, const String& rhs);

public:
    String() : data_(new char[1]{'\0'}) {}

    String(const char* rhs) : data_(new char[strlen(rhs) + 1]) {
        strcpy(data_, rhs);
    }

    // String(const String& rhs) : data_(new char[rhs.size() + 1]) {
    //     strcpy(data_, rhs.c_str());
    // }

    String(const String& rhs) : String(rhs.data_) {}

    ~String() {
        delete[] data_;
    }

    // String& operator= (const String& rhs) {
    //     String tmp(rhs);
    //     swap(tmp);
    //     return *this;
    // }

    String& operator= (String rhs) {
        swap(rhs);
        return *this;
    }

    // String& operator= (const char* rhs) {
    //     return operator=(std::move(String(rhs)));
    // }

    String(String&& rhs) : data_(rhs.data_) {
        rhs.data_ = nullptr;
    }

    // String& operator= (String&& rhs) {
    //     std::cout << "String& operator= (String&& rhs)\n";
    //     swap(rhs);
    //     return *this;
    // }

    const char* c_str() const {
        return data_;
    }

    size_t size() const {
        return strlen(data_);
    }

    void swap(String& rhs) {
        std::swap(data_, rhs.data_);
    }

private:
    char* data_;
};

std::ostream& operator<< (std::ostream& os, const String& rhs) {
    return os << rhs.data_;
}

} // namespace codebase


#endif // CODEBASE_STL_STRING_H_