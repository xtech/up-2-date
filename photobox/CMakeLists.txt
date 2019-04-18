cmake_minimum_required(VERSION 3.7)
project(update_server)

set(CMAKE_CXX_STANDARD 14)

# Generate protobuf files on file change
add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/src/protobuf/api.pb.cc ${CMAKE_CURRENT_SOURCE_DIR}/src/protobuf/api.pb.h
        DEPENDS api.proto
        COMMAND /bin/sh ${CMAKE_CURRENT_SOURCE_DIR}/compile_protobuf.sh
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})


find_package(Protobuf REQUIRED)

add_executable(update_server
        update_server.cpp
        src/protobuf/api.pb.cc src/protobuf/api.pb.h)

include_directories(dependencies/served/src)
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${Protobuf_INCLUDE_DIR})

OPTION (SERVED_BUILD_TESTS "Build served unit test suite" OFF)
OPTION (SERVED_BUILD_EXAMPLES "Build served  examples" OFF)
add_subdirectory(dependencies/served)

target_link_libraries(update_server sfml-system sfml-graphics sfml-window served protobuf)
