#include "ThreadPool.h"

#include <iostream>

namespace concurrent {

ThreadPool::ThreadPool(int workerNum) : exit_(false) {
    workerNum_ = workerNum <= 0 ? std::thread::hardware_concurrency() - 1 : workerNum;
    pool_.reserve(workerNum_);
    for (int i = 0; i < workerNum_; ++i) {
        // pool_[i] = std::thread(&ThreadPool::threadLoop, this); 这是错误的！！！
        pool_.emplace_back(&ThreadPool::threadLoop, this);
    }
}

ThreadPool::~ThreadPool() {
    {
        std::lock_guard<std::mutex> lock(mu_);
        exit_ = true;
    }
    cond_.notify_all();
    for (auto& worker : pool_) {
        worker.join();
    }
}

void ThreadPool::threadLoop() {
    Task task;
    int preQueueSize = 0;
    while (true) {
        {
            std::unique_lock<std::mutex> lock(mu_);
            cond_.wait(lock, [this]{ return !queue_.empty() || exit_; });
            if (exit_) {
                return ;
            }
            preQueueSize = queue_.size();
            task = std::move(queue_.front());
            queue_.pop();
        }

        task();
        {
            std::lock_guard<std::mutex> lock(mu_);
            if (queue_.empty() && preQueueSize == 1) {
                doneCond_.notify_all();
            } 
        }
    }
}

void ThreadPool::wait() {
    std::unique_lock<std::mutex> lock(mu_);
    doneCond_.wait(lock, [this]{ return queue_.empty(); });
}


} // namespace concurrent