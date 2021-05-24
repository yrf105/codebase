cmake_minimum_required(VERSION 3.1.0)

project(hello_headers)

add_executable(hello_headers src/main.cc src/Hello.cc)

target_include_directories(hello_headers
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)
