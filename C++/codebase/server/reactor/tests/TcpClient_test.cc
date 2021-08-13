#include "../TcpClient.h"
#include "../EventLoop.h"
#include "../InetAddress.h"
#include "../Buffer.h"

#include <iostream>

int main() {
    tihi::EventLoop loop;
    tihi::InetAddress serverAddr("39.100.72.123", 80);
    tihi::TcpClient tcpClient(&loop, serverAddr);
    tcpClient.setConnectionCallback([](const tihi::TcpConnection::SPtr& conn){
        std::cout << conn->name() << " 连接\n";
        std::string httpMsg = "GET / HTTP/1.1\r\n\r\n";
        conn->send(httpMsg);
    });
    tcpClient.setMessageCallback([](const tihi::TcpConnection::SPtr& conn, tihi::Buffer* buf, tihi::Timer::TimePoint tp){
        std::cout << tp.time_since_epoch().count() << " 收到 " << conn->peerAddr().toHostPort() << " 的消息: " << buf->retrieveAsString() << std::endl;
    });

    tcpClient.connect();
    loop.loop();

    return 0;
}