#include "../TcpServer.h"

#include "../EventLoop.h"
#include "server/reactor/TcpConnection.h"

// 最基础的功能测试
static void Basic_test() {
    tihi::EventLoop loop;
    tihi::InetAddress listenAddr(12345);
    tihi::TcpServer server(&loop, listenAddr, "basic");
    server.setConnectionCallback([](const tihi::TcpConnection::SPtr& conn) {
        if (conn->connected()) {
            std::cout << conn->peerAddr().toHostPort() << " 连接" << std::endl;
        } else {
            std::cout << conn->peerAddr().toHostPort() << " 断开连接"
                      << std::endl;
        }
    });
    server.setMessageCallback([](const tihi::TcpConnection::SPtr& conn,
                                 tihi::Buffer* buf, tihi::Timer::TimePoint tp) {
        std::cout << tp.time_since_epoch().count() << " " << conn->name()
                  << " 收到消息:\n"
                  << buf->retrieveAsString() << std::endl;
    });

    server.setThreadNum(4);
    server.start();
    loop.loop();
}

// echo
static void ECHO_test() {
    tihi::EventLoop loop;
    tihi::InetAddress serveraddr("127.0.0.1", 12345);
    tihi::TcpServer server(&loop, serveraddr, "ECHO");
    server.setConnectionCallback([](const tihi::TcpConnection::SPtr& conn){
        if (conn->connected()) {
            std::string msg = "Hello " + conn->peerAddr().toHostPort() + '\n';
            conn->send(msg);
        }
    });
    server.setMessageCallback([](const tihi::TcpConnection::SPtr& conn, tihi::Buffer* buf, tihi::Timer::TimePoint tp){
        std::string msg = buf->retrieveAsString();
        std::cout << "收到 " << conn->peerAddr().toHostPort() << " 的消息：" << msg;
        conn->send(msg);
    });

    server.start();
    loop.loop();
}

// 发送两条给定长度的消息
static void TwoMessage_test(int msg1Len, int msg2Len) {
    std::string msg1(msg1Len, 'A');
    std::string msg2(msg2Len, 'B');

    tihi::EventLoop loop;
    tihi::InetAddress serverAddr("127:0.0.1", 12345);
    tihi::TcpServer server(&loop, serverAddr, "TwoMessage");
    server.setConnectionCallback([&msg1, &msg2](const tihi::TcpConnection::SPtr& conn){
        if (conn->connected()) {
            std::cout << conn->name() << " 连接 " << conn->peerAddr().toHostPort() << std::endl;
            if (!msg1.empty()) {
                conn->send(msg1);
            }
            if (!msg2.empty()) {
                conn->send(msg2);
            }
        } else {
            std::cout << conn->name() << " 断开连接 " << conn->peerAddr().toHostPort() << std::endl;
        }
    });
    server.setMessageCallback([](const tihi::TcpConnection::SPtr& conn, tihi::Buffer* buf, tihi::Timer::TimePoint tp){
        std::cout << tp.time_since_epoch().count() << " " << conn->peerAddr().toHostPort() << " " << buf->retrieveAsString();
    });

    server.start();
    loop.loop();
}

std::string message;

// 字符生成服务
static void CharGen_test() {
    int threadNum = 1;
    std::string line;
    for (int i = 33; i < 127; ++i) {
        line.push_back(char(i));
    }
    line += line;

    for (int i = 0; i < 127 - 33; ++i) {
        // 72? https://www.ruanyifeng.com/blog/2011/10/characters_per_line.html
        // 每行字符数的起源
        message += line.substr(i, 72) + '\n';
    }

    tihi::EventLoop loop;
    tihi::InetAddress serverAddr("127.0.0.1", 12345);
    tihi::TcpServer server(&loop, serverAddr, "CharGen_test");

    server.setConnectionCallback([](const tihi::TcpConnection::SPtr& conn) {
        if (conn->connected()) {
            std::cout << conn->name() << " 连接 "
                      << conn->peerAddr().toHostPort() << std::endl;
            conn->send(message);
        } else {
            std::cout << conn->name() << " 断开连接 "
                      << conn->peerAddr().toHostPort() << std::endl;
        }
    });
    server.setWriteCompleteCallback(
        [](const tihi::TcpConnection::SPtr& conn) { conn->send(message); });
    server.setMessageCallback([](const tihi::TcpConnection::SPtr& conn,
                                 tihi::Buffer* buf, tihi::Timer::TimePoint tp) {
        std::cout << tp.time_since_epoch().count() << " "
                  << " 收到：" << conn->peerAddr().toHostPort() << " 的消息："
                  << buf->retrieveAsString();
    });
    server.setThreadNum(threadNum);

    server.start();
    loop.loop();
}

int main(int argc, char** argv) {
    // Basic_test();

    // CharGen_test();

    // if (argc < 3) {
    //     std::cout << "Usage: " << argv[0] << " msg1Len" << " msg2Len" << std::endl;
    //     exit(1);
    // }

    // TwoMessage_test(std::atoi(argv[1]), std::atoi(argv[2]));

    ECHO_test();

    return 0;
}
