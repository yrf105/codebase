set(REACTOR_SRC
    EventLoop.cc
    Channel.cc
    EPoller.cc
    Timer.cc
    TimerQueue.cc
    EventLoopThread.cc
    Acceptor.cc
    Socket.cc
    InetAddress.cc
    SocketsOps.cc
    TcpServer.cc
    TcpConnection.cc
)

set(REACTOR_LIB
    pthread
    spdlog::spdlog
)

add_library(reactor ${REACTOR_SRC})

add_subdirectory(tests)