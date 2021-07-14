#ifndef CODEBASE_UTILS_NONCOPYABLE_H
#define CODEBASE_UTILS_NONCOPYABLE_H

namespace codebase {

class Noncopyable {
protected:
    Noncopyable() = default;
    ~Noncopyable() = default;

private:
    Noncopyable(const Noncopyable&) = delete;
    Noncopyable& operator= (const Noncopyable&) = delete;
};

} // namespace codebase


#endif // CODEBASE_UTILS_NONCOPYABLE_H