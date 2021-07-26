set(REACTOR_SRC
    EventLoop.cc
)

set(REACTOR_LIB
    pthread
    spdlog::spdlog
)

add_library(reactor ${REACTOR_SRC})

add_subdirectory(tests)