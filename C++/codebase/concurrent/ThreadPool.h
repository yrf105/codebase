// https://github.com/cbraley/threadpool/blob/master/src/thread_pool.cc
// https://blog.csdn.net/libaineu2004/article/details/79612872
// https://github.com/mtrebi/thread-pool
// https://www.zhihu.com/question/27908489/answer/355105668

#include <boost/noncopyable.hpp>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>
#include <queue>
#include <vector>

namespace concurrent {

class ThreadPool : public boost::noncopyable {
public:
    using Task = std::function<void()>;
    using TaskQueue = std::queue<Task>;
    using Pool = std::vector<std::thread>;

    explicit ThreadPool(int workerNum = 0);
    ~ThreadPool();

    template <typename Task>
    void schedule(Task&& task) {
        if (workerNum_ > 0) {
            {
                std::lock_guard<std::mutex> lock(mu_);
                queue_.emplace(std::forward<Task>(task));
            }
            cond_.notify_one();
        } else {
            task();
        }
    }

    void wait();

private:
    void threadLoop();

private:
    bool exit_;
    int workerNum_;
    mutable std::mutex mu_;
    std::condition_variable cond_;
    std::condition_variable doneCond_;
    TaskQueue queue_;
    Pool pool_;
};

}  // namespace concurrent