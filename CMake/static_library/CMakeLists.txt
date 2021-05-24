cmake_minimum_required(VERSION 3.1.0)

project(static_binary)

add_library(static_library STATIC
    src/Hello.cc
)

target_include_directories(static_library
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

add_executable(hello_binary src/main.cc)
target_link_libraries(hello_binary static_library)