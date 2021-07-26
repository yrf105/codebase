add_executable(log_test log_test.cc)
target_link_libraries(log_test PRIVATE spdlog::spdlog)

add_executable(EventLoop_test01 EventLoop_test01.cc)
target_link_libraries(EventLoop_test01 reactor pthread spdlog::spdlog)

add_executable(EventLoop_test02 EventLoop_test02.cc)
target_link_libraries(EventLoop_test02 reactor pthread)

add_executable(EventLoop_test03 EventLoop_test03.cc)
target_link_libraries(EventLoop_test03 reactor pthread)