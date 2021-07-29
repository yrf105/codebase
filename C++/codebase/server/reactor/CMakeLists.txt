set(REACTOR_SRC
    EventLoop.cc
    Channel.cc
    EPoller.cc
    Timer.cc
    TimerQueue.cc
    EventLoopThread.cc
)

set(REACTOR_LIB
    pthread
    spdlog::spdlog
)

add_library(reactor ${REACTOR_SRC})

add_subdirectory(tests)