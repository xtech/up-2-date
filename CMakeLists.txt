cmake_minimum_required(VERSION 3.7)
project(up_2_date)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)
include_directories(dependencies/served/src)

OPTION (SERVED_BUILD_TESTS "Build served unit test suite" OFF)
OPTION (SERVED_BUILD_EXAMPLES "Build served  examples" OFF)
add_subdirectory(dependencies/served)


add_executable(up2date src/main.cpp)

target_link_libraries(up2date served)

