// [官方文档]（https://google.github.io/googletest/）

// [使用GTEST编写C++测试用例进阶教程（GTEST
// advanced中文译文）](https://www.jianshu.com/p/215edbfc2e0a)
// [GoogleTest FAQ 中文译文](https://www.jianshu.com/p/1153b9723716)

// [Google
// C++单元测试框架GoogleTest(汇总)](https://www.cnblogs.com/jycboy/p/gtest_catalog.html)

#include <gtest/gtest.h>

#include <queue>

// [如何在GoogleTest中运行特定的测试用例？](https://cloud.tencent.com/developer/ask/108663)
// 只运行这一个测试 ../bin/basic_test --gtest_filter=HelloTest.BasicAssertions
// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
    // Expect two strings not to be equal.
    EXPECT_STRNE("hello", "world");
    // Expect equality.
    EXPECT_EQ(7 * 6, 42);
}

static int gcd(int a, int b) {
    if (a == 0 || b == 0) {
        return 0;
    }

    int c = a % b;
    if (c == 0) {
        return b;
    }
    return gcd(b, c);
}

TEST(BASIC, gcd) {
    EXPECT_EQ(3, gcd(9, 3));
    EXPECT_EQ(6, gcd(36, 6));
}

TEST(BASIC, CUSTOM_FAILURE_MESSAGE) {
    int a1 = 4;
    int b1 = 8;
    int a2 = 9;
    int b2 = 3;

    int c1 = gcd(a1, b1);
    int c2 = gcd(a2, b2);

    EXPECT_EQ(c1, c2) << a1 << " 和 " << b1 << " 的 gcd 是 " << c1 << ", " << a2
                      << " 和 " << b2 << " 的 gcd 是 " << c2;
}

class queueTest : public ::testing::Test {

protected:

    // void SetUp() override {
    queueTest() : q0_(), q1_(), q2_() {
        // q0_.push(1);
        // q1_.push(2);
        // q2_.push(3);
    }

    // void TearDown() override {
    ~queueTest() override {

    }

    std::queue<int> q0_;
    std::queue<int> q1_;
    std::queue<int> q2_;
};

TEST_F(queueTest, IsEmptyInitially) {
    EXPECT_EQ(q0_.size(), 0);
}



int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    
    return RUN_ALL_TESTS();
}