#include "../EventLoop.h"
#include <iostream>
#include "../EventLoopThread.h"
#include "../Acceptor.h"
#include <iostream>
#include "../SocketsOps.h"

static void Test_runInLoop() {
    tihi::EventLoop loop;

    loop.runInLoop([&loop]{
        std::cout << "test runInLoop()" << std::endl;
        loop.runAfter(std::chrono::seconds(2), [&loop]{
            std::cout << "runAfter" << std::endl;
            loop.queueInLoop([&loop]{
                std::cout << "queueInLoop" << std::endl;
                loop.quit();
            });
        });
    });

    loop.loop();
}

static void Test_threadSafeAddTimer() {
    tihi::EventLoop loop;
    std::thread t1([&loop]{
        std::cout << "在其他线程中向 IO 线程添加定时器" << std::endl;
        loop.runAfter(std::chrono::seconds(1), [&loop]{
            loop.quit();
        });
    });
    loop.loop();
    t1.join();
}

static void Test_EventLoopThread() {
    std::cout << "main " << std::this_thread::get_id() << std::endl;
    tihi::EventLoopThread eLoopThread;
    tihi::EventLoop* loop = eLoopThread.startLoop();
    loop->runInLoop([]{
        std::cout << "EventLoopThread " << std::this_thread::get_id() << std::endl;
    });
    std::this_thread::sleep_for(std::chrono::seconds(1));
    loop->runAfter(std::chrono::seconds(2), []{
        std::cout << "EventLoopThread " << std::this_thread::get_id() << std::endl;
    });
    std::this_thread::sleep_for(std::chrono::seconds(3));
    loop->quit();
}

static void Test_Acceptor() {
    tihi::EventLoop loop;
    tihi::InetAddress listenAddr(12345);

    tihi::Acceptor acceptor(&loop, listenAddr);
    acceptor.setNewConnectionCallback([](int sockfd, const tihi::InetAddress& address){
        char buf[30] = {0};
        tihi::sockets::toHostPort(buf, sizeof(buf), address.getSockAddrInet());
        std::cout << "收到来自 " << buf << " 的消息" << std::endl;
        char message[] = "hello\n";
        ::write(sockfd, message, sizeof(message));
        tihi::sockets::close(sockfd);
    });
    acceptor.listen();

    loop.loop();
}

int main(int argc, char** argv) {

    // Test_runInLoop();
    // Test_threadSafeAddTimer();
    // Test_EventLoopThread();
    Test_Acceptor();

    return 0;
}