cmake_minimum_required(VERSION 3.10)
project(TS_Stack)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(ts_stack_lib STATIC
    src/producer.cpp
    src/consumer.cpp
)

target_include_directories(ts_stack_lib
    PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

find_package(Threads REQUIRED)
target_link_libraries(ts_stack_lib
    Threads::Threads
)

option(ENABLE_VISUALIZE "Enable visualization output" OFF)

if(ENABLE_VISUALIZE)
    target_compile_definitions(ts_stack_lib PRIVATE VISUALIZE)
    target_compile_definitions(ts_stack_lib PUBLIC VISUALIZE)
    message(STATUS "Visualization ENABLED")
else()
    message(STATUS "Visualization DISABLED")
endif()

add_executable(ts_stack
    src/main.cpp
)

target_link_libraries(ts_stack
    ts_stack_lib
)

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
