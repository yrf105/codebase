#include "Thread.h"

namespace codebase {

namespace server {

namespace object_oriented {

Thread::Thread() {}

Thread::~Thread() {}

void Thread::start() {
    ::pthread_create(&threadId_, nullptr, ThreadRoutine, this);
}

void Thread::join() {
    ::pthread_join(threadId_, nullptr);
}

void* Thread::ThreadRoutine(void* args) {
    Thread* self = static_cast<Thread*>(args);
    self->run();
    if (self->autoDelete_) {
        delete self;
    }

    return nullptr;
}

} // namespace object_oriented

namespace object_based {

Thread::Thread(const std::function<void()>& cb) : cb_(cb) {}

Thread::~Thread() {}

void Thread::start() {
    pthread_create(&threadId_, nullptr, threadRoutine, this);
}

void Thread::join() {
    pthread_join(threadId_, nullptr);
}

void* Thread::threadRoutine(void* args) {
    Thread* self = static_cast<Thread*>(args);
    self->cb_();
    return nullptr;
}

void Thread::run() {}

} // namepsace object_based

} // namespace server

} // namespace codebase