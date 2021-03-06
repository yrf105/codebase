cmake_minimum_required(VERSION 3.16.0)
project(codebase)

include(cmake/utils.cmake)

# 显示详细的 make 命令
# set(CMAKE_VERBOSE_MAKEFILE ON)

#判断编译器类型,如果是gcc编译器,则在编译选项中加入c++11支持
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O0 -ggdb -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined -std=c++2a ${CMAKE_CXX_FLAGS}")
    # message(STATUS "optional:-std=c++11")
endif(CMAKE_COMPILER_IS_GNUCXX)

# 定义工程根目录; CMAKE_SOURCE_DIR为内建变量，表示工程根目录的CMakeLists.txt文件路径
SET(ROOT_DIR ${CMAKE_SOURCE_DIR})

# 指定头文件搜索路径
include_directories(codebase/)
# include_directories(third_party/yaml-cpp/include)

# 指定引用的外部库的路径
# link_directories(third_party/yaml-cpp/bulid)

if(NOT TARGET spdlog)
    # Stand-alone build
    find_package(spdlog REQUIRED)
endif()

set(LIB_SRC
    codebase/random_number/random_number.cc
    codebase/utils/utils.cc
    codebase/utils/noncopyable.cc
    codebase/design_patterns/singleton.cc
    codebase/STL/List.cc
    codebase/datastructure/lock_free_queue.cc
    codebase/STL/String.cc
    codebase/STL/Vector.cc
    codebase/STL/smart_ptr.cc
    codebase/STL/RefCounter.cc
    codebase/server/Thread.cc)

set(LIBS
    codebase
    pthread)

# 生成动态库
add_library(codebase SHARED ${LIB_SRC})
redefine_file_macro(codebase)

# 生成可执行文件并添加依赖
codebase_add_executable(test "tests/test.cc" codebase "${LIBS}")
# codebase_add_executable(test_random_number "tests/test_random_number.cc" codebase "${LIBS}")
# codebase_add_executable(test_stl "tests/test_stl.cc" codebase "${LIBS}")
# codebase_add_executable(test_allocator "tests/test_allocator.cc" codebase "${LIBS}")
# codebase_add_executable(test_function_bind "tests/test_function_bind.cc" codebase "${LIBS}")
# codebase_add_executable(test_ucontext "tests/test_ucontext.cc" codebase "${LIBS}")
# # codebase_add_executable(test_coroutine "tests/test_coroutine.cc" codebase "${LIBS}")
# codebase_add_executable(test_tuple "tests/test_tuple.cc" codebase "${LIBS}")
# codebase_add_executable(singleton_test "tests/singleton_test.cc" codebase "${LIBS}")
# codebase_add_executable(mmap_test "tests/mmap_test.cc" codebase "${LIBS}")
# codebase_add_executable(List_test "tests/List_test.cc" codebase "${LIBS}")
# codebase_add_executable(lock_free_queue_test "tests/lock_free_queue_test.cc" codebase "${LIBS}")
# codebase_add_executable(test_virtual_func "tests/test_virtual_func.cc" codebase "${LIBS}")
# codebase_add_executable(test_String "tests/test_String.cc" codebase "${LIBS}")
# codebase_add_executable(test_Vector "tests/test_Vector.cc" codebase "${LIBS}")
# codebase_add_executable(test_smart_ptr "tests/test_smart_ptr.cc" codebase "${LIBS}")
# codebase_add_executable(test_server_Thread "tests/test_server_Thread.cc" codebase "${LIBS}")
# codebase_add_executable(test_equality_comparable "tests/test_equality_comparable.cc" codebase "${LIBS}")
# codebase_add_executable(noncopyable_test "tests/noncopyable_test.cc" codebase "${LIBS}")
# codebase_add_executable(bitset_test "tests/bitset_test.cc" codebase "${LIBS}")
# codebase_add_executable(explicit_test "tests/explicit_test.cc" codebase "${LIBS}")

# 指定可执行文件的存放路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# 指定输出库的存放路径
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

add_subdirectory(codebase/concurrent)
add_subdirectory(codebase/basic)
add_subdirectory(codebase/server)
# add_subdirectory(codebase/effective)
# add_subdirectory(codebase/moreeffective)
add_subdirectory(codebase/effectivemodern)
add_subdirectory(codebase/log)

add_subdirectory(tests/googletest_examples)
add_subdirectory(tests/benchmark_examples)