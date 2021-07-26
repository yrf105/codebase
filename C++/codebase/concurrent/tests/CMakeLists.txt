add_executable(c++11_thread_pooled_test c++11_thread_pooled_test.cc)
target_link_libraries(c++11_thread_pooled_test pthread)

include(FetchContent)
set(FETCHCONTENT_QUIET off) # 使 git clone 信息可见
get_filename_component(fc_base "/home/wddxrw/app"
                       REALPATH BASE_DIR "${CMAKE_BINARY_DIR}")
set(FETCHCONTENT_BASE_DIR ${fc_base})

FetchContent_Declare(
  readerwriterqueue
  GIT_REPOSITORY    https://gitee.com/yue_rui_feng/readerwriterqueue.git
  GIT_TAG           master
)

FetchContent_MakeAvailable(readerwriterqueue)

find_package(benchmark REQUIRED)

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(readerwriterqueue_test readerwriterqueue_test.cc)
target_link_libraries(readerwriterqueue_test pthread readerwriterqueue)

add_executable(lauch_thread_performance_test lauch_thread_performance_test.cc ../../basic/doSomething.cc)
target_link_libraries(lauch_thread_performance_test pthread readerwriterqueue)

add_executable(concurrentqueue_test concurrentqueue_test.cc)
target_link_libraries(concurrentqueue_test pthread)

add_executable(ThreadPool_test ThreadPool_test.cc ../ThreadPool.cc)
target_link_libraries(ThreadPool_test gtest pthread)

add_executable(ThreadPool_benchmark ThreadPool_benchmark.cc ../ThreadPool.cc)
target_link_libraries(ThreadPool_benchmark pthread benchmark::benchmark)