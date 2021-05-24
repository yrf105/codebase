cmake_minimum_required(VERSION 3.1.0)

project(shared_binary)

add_library(shared_library SHARED
    src/Hello.cc
)

target_include_directories(shared_library
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

add_library(hello::library
    ALIAS
        shared_library
)

add_executable(hello_binary src/main.cc)
target_link_libraries(hello_binary
    PRIVATE
        hello::library
)
