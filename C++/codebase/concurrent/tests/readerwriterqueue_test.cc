// https://github.com/cameron314/readerwriterqueue
// 一个速度飞快的读写队列（单读者，单写者）

#include "assert.h"

#include <readerwriterqueue.h>
#include <readerwritercircularbuffer.h>
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    using namespace moodycamel;

    { // 非阻塞版
        // 预留出 100 个位子
        ReaderWriterQueue<int> q(100);
        
        // 若队列满，会分配内存
        q.enqueue(99);

        // 只有当队列为空时才会成功（不会分配内存）
        bool succeeded = q.try_enqueue(100);
        assert(succeeded);


        int number;
        succeeded = q.try_dequeue(number);
        assert(succeeded && number == 99);

        // 使用 peek 访问队首元素
        int *front = q.peek();
        assert(*front == 100);
        succeeded = q.try_dequeue(number);
        assert(succeeded && number == 100);
        front = q.peek();
        // 若队列为空 peek 返回 nullptr
        assert(front == nullptr);
        succeeded = q.try_dequeue(number);
        // 若队列为空会返回 false
        assert(!succeeded);
        // number 值不变
        assert(number == 100);
    }


    { // 阻塞版
        BlockingReaderWriterQueue<int> q;
        int item;
    #if 0
        std::thread reader([&]{
            for (int i = 0; i < 100; ++i) {
                // 若队列一直为空则一直阻塞下去
                // 所以当使用它时，要确保会有新的元素加入队列
                // 若队列在阻塞期间被析构，会出现未定义行为
                q.wait_dequeue(item);
            }
        }); // 最终 size 一定为 0
    #else
        std::thread reader([&]{
            for (int i = 0; i < 100;) {
                // 不会一直阻塞，超时后会自动唤醒
                q.wait_dequeue_timed(item, std::chrono::milliseconds(5));
                ++i;
            }
        }); // 最终 size 不固定
    #endif

        std::thread writer([&]{
            for (int i = 0; i < 100; ++i) {
                q.enqueue(i);
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        });

        reader.join();
        writer.join();

        // assert(q.size_approx() == 0);
        std::cout << q.size_approx() << std::endl;
    }

    {// 环形缓冲区，基本操作同上，只是槽位数固定
        BlockingReaderWriterCircularBuffer<int> q(1024);
    }

    return 0;
}