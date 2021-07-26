#include "../ThreadPool.h"

#include <assert.h>
#include <gtest/gtest.h>

#include <chrono>
#include <iostream>
#include <memory>

class ThreadPoolTest : public ::testing::Test {
protected:
    ThreadPoolTest() : poolUPtr_(new concurrent::ThreadPool) {
    }
    ~ThreadPoolTest() override {}

    std::unique_ptr<concurrent::ThreadPool> poolUPtr_;
};

TEST_F(ThreadPoolTest, BasicUsage) {
    std::vector<int> vec(1000, 0);

    if (poolUPtr_ == nullptr) {
        std::cout << "start\n";
    }

    for (int i = 0; i < 1000; ++i) {
        poolUPtr_->schedule([&vec, i] { ++vec[i]; });
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));

    for (auto i : vec) {
        EXPECT_EQ(i, 1);
    }
}

TEST_F(ThreadPoolTest, Wait) {
    for (int i = 0; i < 1000; ++i) {
        poolUPtr_->schedule([](){
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        });
    }
    poolUPtr_->wait();
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}