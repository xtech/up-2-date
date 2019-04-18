cmake_minimum_required(VERSION 3.7)
project(update_server)

set(CMAKE_CXX_STANDARD 14)


add_executable(update_server
        update_server.cpp)

include_directories(dependencies/served/src)

OPTION (SERVED_BUILD_TESTS "Build served unit test suite" OFF)
OPTION (SERVED_BUILD_EXAMPLES "Build served  examples" OFF)
add_subdirectory(dependencies/served)

target_link_libraries(update_server sfml-system sfml-graphics sfml-window served)
