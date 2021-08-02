#ifndef REACTOR_BUFFER_H
#define REACTOR_BUFFER_H

#include <vector>
#include <assert.h>
#include <string>
#include <iostream>

namespace tihi {

class Buffer {
public:
    static const size_t kCheapPrepend = 8;
    static const size_t kInitialSize = 1024;

    Buffer()
        : buffer_(kCheapPrepend + kInitialSize),
          readerIndex_(kCheapPrepend),
          writerIndex_(kCheapPrepend) {
        assert(readableBytes() == 0);
        assert(writableBytes() == kInitialSize);
        assert(prependableBytes() == kCheapPrepend);
    } 

    size_t readableBytes() const {
        return writerIndex_ - readerIndex_;
    }
    size_t writableBytes() const {
        return buffer_.size() - writerIndex_;
    }
    size_t prependableBytes() const {
        return readerIndex_;
    }

    // 获取可读区域（已写入区域）的头指针
    const char* peek() const {
        return begin() + readerIndex_;
    }

    // 获取可写入区域的头指针
    char* beginWrite() {
        return begin() + writerIndex_;
    }

    const char* beginWrite() const {
        return begin() + writerIndex_;
    }

    // 从缓冲中读走 len 字节
    // len <= 可读字节数
    void retrieve(size_t len) {
        assert(len <= readableBytes());
        readerIndex_ += len;
    }

    // 从缓冲中读走 [peek(), end)
    // end in [peek(), beginWrite()]
    void retrieveUntil(const char* end) {
        assert(peek() <= end);
        assert(end <= beginWrite());
        retrieve(end - peek());
    }

    // 将已写入的数据清空
    void retrieveAll() {
        readerIndex_ = kCheapPrepend;
        writerIndex_ = kCheapPrepend;
    }

    std::string retrieveAsString() {
        std::string ret(peek(), readableBytes());
        retrieveAll();
        return ret;
    }

    void append(const std::string& data, size_t len) {
        append(data.data(), data.size());
    }

    void append(const void* data, size_t len) {
        append(static_cast<const char*>(data), len);
    }

    void append(const char* data, size_t len) {
        ensureWritableBytes(len);
        std::copy(data, data + len, beginWrite());
        hasWritten(len);
    }

    void ensureWritableBytes(size_t len) {
        if (writerIndex_ + len < buffer_.size()) {
            makeSpace(len);
        }
        assert(writableBytes() >= len);
    }

    void hasWritten(size_t len) {
        writerIndex_ += len;
    }

    void prepend(const void* data, size_t len) {
        assert(len <= prependableBytes());
        readerIndex_ -= len;
        const char* d = static_cast<const char*>(data);
        std::copy(d, d + len, begin() + readerIndex_);
    }

    // 将 buffer 的可写区域缩小至指定容量
    void shrink(size_t reserve) {
        std::vector<char> buf(kCheapPrepend + readableBytes() + reserve);
        std::copy(peek(), peek() + readableBytes(), buf.begin() + kCheapPrepend);
        buf.swap(buffer_);
    }

    ssize_t readFd(int fd, int* savedErrno);

private:
    char* begin() {
        return buffer_.data();
    }

    const char* begin() const {
        return buffer_.data();
    }

    void makeSpace(size_t len) {
        if (readerIndex_ + writableBytes() < len + kCheapPrepend) {
            buffer_.resize(len + writerIndex_);
        } else {
            // 往前移动可读区域，腾出可写区域
            size_t readable = readableBytes();
            std::copy(begin() + readerIndex_,
                      begin() + writerIndex_,
                      begin() + kCheapPrepend);
            readerIndex_ = kCheapPrepend;
            writerIndex_ = readerIndex_ + readable;
            assert(writableBytes() == readable);
        }
    }

private:
    std::vector<char> buffer_;
    int readerIndex_;
    int writerIndex_;
};

}  // namespace tihi

#endif  // REACTOR_BUFFER_H