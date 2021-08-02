#include "Buffer.h"

#include <sys/uio.h>

namespace tihi {

ssize_t Buffer::readFd(int fd, int* savedErrno) {
    char extraBuf[65536];
    iovec iov[2];
    size_t writable = writableBytes();
    iov[0].iov_base = beginWrite();
    iov[0].iov_len = writable;
    iov[1].iov_base = extraBuf;
    iov[1].iov_len = sizeof(extraBuf);
    ssize_t n = ::readv(fd, iov, 2);
    if (n < 0) {
        *savedErrno = errno;
    } else if (static_cast<size_t>(n) <= writable) {
        hasWritten(n);
    } else {
        writerIndex_ = buffer_.size();
        append(extraBuf, n - writable);
    }
    return n;
}

} // namespace tihi