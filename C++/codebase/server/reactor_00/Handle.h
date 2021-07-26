#ifndef CODEBASE_SERVER_REACTOR_HANDLE_H
#define CODEBASE_SERVER_REACTOR_HANDLE_H

#include <memory>
#include <boost/noncopyable.hpp>

namespace codebase {

class Handle : public boost::noncopyable {
public:
    using SPtr = std::shared_ptr<Handle>;
    explicit Handle(int fd);
    virtual ~Handle();

    int fd() const {
        return fd_;
    }

private:
    int fd_;
};

} // namespace codebase

#endif // CODEBASE_SERVER_REACTOR_HANDLE_H