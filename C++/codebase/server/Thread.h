#ifndef CODEBASE_SERVER_THREAD_H
#define CODEBASE_SERVER_THREAD_H

#include <pthread.h>

#include <functional>

namespace codebase {

namespace server {

namespace object_oriented {
class Thread {

public:
    Thread();
    virtual ~Thread();

    void start();
    void join();
    void setAutoDelete(bool autoDelete) { autoDelete_ = autoDelete; }

private:
    static void* ThreadRoutine(void* args);
    virtual void run() = 0;

private:
    pthread_t threadId_;
    bool autoDelete_ = false;
};

} // namespace object_oriented

namespace object_based {

class Thread {
public:
    Thread(const std::function<void()>& cb);
    ~Thread();

    void start();
    void join();

private:
    static void* threadRoutine(void* args);
    void run();

private:
    pthread_t threadId_;
    std::function<void()> cb_;
};

} // namespace object_based

} // namespace server

} // namespace codebase

#endif // CODEBASE_SERVER_THREAD_H