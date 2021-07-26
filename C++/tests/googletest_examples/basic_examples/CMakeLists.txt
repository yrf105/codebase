enable_testing()

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(basic_test basic_test.cc)
target_link_libraries(basic_test gtest pthread)
# include(GoogleTest)
# gtest_discover_tests(basic_test)