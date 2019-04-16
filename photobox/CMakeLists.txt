cmake_minimum_required(VERSION 3.7)
project(update_server)

set(CMAKE_CXX_STANDARD 14)

add_executable(update_server
        update_server.cpp)

target_link_libraries(update_server sfml-system sfml-graphics sfml-window)
