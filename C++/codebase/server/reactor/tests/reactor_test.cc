#include "../EventLoop.h"
#include <iostream>
#include "../EventLoopThread.h"
#include "../Acceptor.h"
#include <iostream>
#include "../SocketsOps.h"
#include "../TcpServer.h"
#include "../Connector.h"

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

static void Test_AcceptorTowPort() {
    tihi::EventLoop loop;
    tihi::InetAddress addr1(12345);
    tihi::InetAddress addr2(13254);

    tihi::Acceptor acceptor1(&loop, addr1);
    tihi::Acceptor acceptor2(&loop, addr2);

    acceptor1.setNewConnectionCallback([](int sockfd, const tihi::InetAddress& address){
        char buf[30] = {0};
        tihi::sockets::toHostPort(buf, sizeof(buf), address.getSockAddrInet());
        std::cout << "收到来自 " << buf << " 的消息" << std::endl;
        char message[] = "hello\n";
        ::write(sockfd, message, sizeof(message));
        tihi::sockets::close(sockfd);
    });

    acceptor2.setNewConnectionCallback([](int sockfd, const tihi::InetAddress& address){
        char buf[30] = {0};
        tihi::sockets::toHostPort(buf, sizeof(buf), address.getSockAddrInet());
        std::cout << "收到来自 " << buf << " 的消息" << std::endl;
        char message[] = "world\n";
        ::write(sockfd, message, sizeof(message));
        tihi::sockets::close(sockfd);
    });
    acceptor1.listen();
    acceptor2.listen();

    loop.loop();
}

tihi::Buffer buf;

static void TEST_TcpServer() {
    tihi::EventLoop loop;
    tihi::InetAddress addr(12345);
    tihi::TcpServer tcpServer(&loop, addr);
    tcpServer.setConnectionCallback([](const tihi::TcpConnection::SPtr& connection){
        std::cout << connection->peerAddr().toHostPort() << " 连接" << std::endl;
    });
    tcpServer.setMessageCallback([](const tihi::TcpConnection::SPtr& connection, tihi::Buffer* buf, std::chrono::system_clock::time_point receiveTimepoint){
        std::cout << buf->retrieveAsString();
    });

    tcpServer.start();
    loop.loop();
}

static void TEST_TcpConnectionSend() {
    tihi::EventLoop loop;
    tihi::InetAddress addr(12345);
    tihi::TcpServer server(&loop, addr);

    std::string message1;
    for (int i = 0; i < 1024 * 1024 + 4; ++i) {
        message1 += "Y";
    }
    std::string message2 = "hello Y";

    server.setConnectionCallback([](const tihi::TcpConnection::SPtr& connection){
        std::cout <<  std::this_thread::get_id() << " " << connection->peerAddr().toHostPort() << " 连接" << std::endl;
    });

    server.setMessageCallback([&message1, &message2](const tihi::TcpConnection::SPtr& connection, tihi::Buffer* buf, std::chrono::system_clock::time_point receiveTimepoint){
        if (connection->connected()) {
            std::cout << std::this_thread::get_id() << " " << buf->retrieveAsString();
            connection->send(message1);
            connection->send(message2);
            connection->shutdown();
        }
    });

    server.setThreadNum(2);
    server.start();
    loop.loop();
}

static void Test_Connector() {
    tihi::InetAddress addr("220.181.38.251", 80);
    tihi::EventLoop loop;
    tihi::Connector connector(&loop, addr);

    connector.setNewConnectionCallback([&loop](int fd){
        std::cout << fd << " 已连接" << std::endl;
        loop.quit();
    });

    connector.start();
    loop.loop();
}

int main(int argc, char** argv) {

    // Test_runInLoop();
    // Test_threadSafeAddTimer();
    // Test_EventLoopThread();
    // Test_Acceptor();
    // Test_AcceptorTowPort();
    // TEST_TcpServer();

    // TEST_TcpConnectionSend();

    Test_Connector();

    return 0;
}