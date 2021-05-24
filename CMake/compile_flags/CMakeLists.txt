cmake_minimum_required(VERSION 3.1)

# Set a default C++ compile flag
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DEX2" CACHE STRING "Set C++ Compiler Flags" FORCE)

project(build_type)

add_executable(cmake_examples_compile_flags hello.cc)

target_compile_definitions(cmake_examples_compile_flags
	PRIVATE EX3
)