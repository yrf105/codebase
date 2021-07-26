// https://github.com/cameron314/concurrentqueue/blob/master/samples.md

#include "concurrentqueue.h"
#include <assert.h>
#include <vector>
#include <thread>
#include <iostream>

int main(int argc, char** argv) {

    using namespace moodycamel;

    { // hello queue
#if 0
        ConcurrentQueue<int> q;
        for (int i = 0; i < 100; ++i) {
            q.enqueue(i);
        }
        int item;
        for (int i = 0; i < 100; ++i) {
            if (q.try_dequeue(item)) {
                assert(item == i);
            }
        }
#endif
    }

    { // Hello concurrency
#if 1
        ConcurrentQueue<int> q;

        std::vector<int> vec(100, 0);
        std::vector<std::thread> thrs;

        for (int i = 0; i < 10; ++i) {
            thrs.push_back(std::thread([&](int i){
                for (int j = 0; j < 10; ++j) {
                    q.enqueue(i * 10 + j);
                }
            }, i));
        }


        // // 这样是不对的
        // for (int i = 0; i < 10; ++i) {
        //     thrs.push_back(std::thread([&](){
        //         std::cout << i << std::endl;
        //         for (int j = 0; j < 10; ++j) {
        //             q.enqueue(i * 10 + j);
        //         }
        //     }));
        // }

        for (int i = 0; i < 10; ++i) {
            thrs.push_back(std::thread([&](){
                int idx;
                for (int j = 0; j < 20; ++j) {
                    if (q.try_dequeue(idx)) {
                        ++vec[idx];
                    }
                }
            }));
        }

        for (auto& t : thrs) {
            t.join();
        }

        int idx;
        while (q.try_dequeue(idx)) {
            ++vec[idx];
        }

        for (auto i : vec) {
            assert(i == 1);
        }

#endif
    }

    return 0;
}