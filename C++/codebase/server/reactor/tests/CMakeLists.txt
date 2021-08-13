add_executable(log_test log_test.cc)
target_link_libraries(log_test PRIVATE spdlog::spdlog)

add_executable(EventLoop_test01 EventLoop_test01.cc)
target_link_libraries(EventLoop_test01 reactor pthread spdlog::spdlog)

add_executable(EventLoop_test02 EventLoop_test02.cc)
target_link_libraries(EventLoop_test02 reactor pthread)

add_executable(EventLoop_test03 EventLoop_test03.cc)
target_link_libraries(EventLoop_test03 reactor pthread)

add_executable(reactor_test01 reactor_test01.cc)
target_link_libraries(reactor_test01 reactor pthread)

add_executable(reactor_test02 reactor_test02.cc)
target_link_libraries(reactor_test02 reactor pthread)

add_executable(reactor_test reactor_test.cc)
target_link_libraries(reactor_test reactor pthread)

add_executable(TcpClient_test TcpClient_test.cc)
target_link_libraries(TcpClient_test reactor pthread)