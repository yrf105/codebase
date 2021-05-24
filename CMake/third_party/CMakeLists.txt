cmake_minimum_required(VERSION 3.16)

project(third_party)

find_package(Boost REQUIRED COMPONENTS system filesystem)

if(BOOST_FOUND)
    message("Boost found")
    include_directories(${Boost_INCLUDE_DIRS})
else()
    message(FATAL ERROR "Cannot find Boost")
endif()

add_executable(third_party hello.cc)
target_link_libraries(third_party
    PRIVATE
        Boost::filesystem
)