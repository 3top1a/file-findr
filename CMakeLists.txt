cmake_minimum_required(VERSION 3.17)
project(file_findr)

set(CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_FLAGS "-lstdc++fs -std=c++17")

include_directories(.)

link_directories(/home/not-a-weeb/jsoncpp/build/debug/lib/)

add_executable(file_findr
        findr.cpp
        findr.h)

target_link_libraries(file_findr libjsoncpp.a)