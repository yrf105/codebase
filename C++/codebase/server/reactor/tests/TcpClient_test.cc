#include "../TcpClient.h"

#include <iostream>

#include "../Buffer.h"
#include "../EventLoop.h"
#include "../InetAddress.h"

#include <vector>
#include <memory>

int NumConnections = 0;

static void Basic_test() {
    tihi::EventLoop loop;
    tihi::InetAddress serverAddr("127.0.0.1", 12345);
    tihi::TcpClient tcpClient(&loop, serverAddr);
    tcpClient.setConnectionCallback([](const tihi::TcpConnection::SPtr& conn) {
        if (conn->connected()) {
            std::cout << conn->name() << " 连接\n";
            std::string httpMsg = "GET / HTTP/1.1\r\n\r\n";
            conn->send(httpMsg);
        } else {
            std::cout << conn->name() << " 断开连接" << std::endl;
        }
    });
    tcpClient.setMessageCallback([](const tihi::TcpConnection::SPtr& conn,
                                    tihi::Buffer* buf,
                                    tihi::Timer::TimePoint tp) {
        std::cout << tp.time_since_epoch().count() << " " << conn->name() << " 收到 "
                  << conn->peerAddr().toHostPort()
                  << " 的消息: " << buf->retrieveAsString() << std::endl;
    });

    tcpClient.connect();
    loop.loop();
}

static void NConnections_test() {
    tihi::EventLoop loop;
    tihi::InetAddress serverAddr("127.0.0.1", 12345);
    std::vector<std::shared_ptr<tihi::TcpClient>> clients;
    loop.runInLoop([&loop, &serverAddr, &clients](){
        for (int i = 0; i < NumConnections; i += 100) {
            for (int j = 0; j < 100; ++j) {
                std::shared_ptr<tihi::TcpClient> client(new tihi::TcpClient(&loop, serverAddr));
                clients.push_back(client);
                client->setConnectionCallback([client](const tihi::TcpConnection::SPtr& conn){
                    if (conn->connected()) {
                        std::cout << conn->name() << " 连接\n";
                        conn->getLoop()->runAfter(std::chrono::seconds(10), std::bind(&tihi::TcpClient::disconnect, client));
                    } else {
                        std::cout << conn->name() << " 断开连接\n";
                    }
                });
                client->connect();
            }
            std::this_thread::sleep_for(std::chrono::microseconds(50));
        }
    });

    loop.loop();
}

int main(int argc, char** argv) {

    if (argc >= 2) {
        NumConnections = std::stoi(argv[1]);
    }

    // Basic_test();
    NConnections_test();
    return 0;
}