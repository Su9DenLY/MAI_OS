cmake_minimum_required(VERSION 3.10)
project(lw4)

include_directories(${CMAKE_SOURCE_DIR})

add_library(function1 SHARED function1.cpp)
add_library(function2 SHARED function2.cpp)

add_executable(static static_main.cpp function.hpp)
add_executable(dynamic dynamic_main.cpp function.hpp)

target_link_libraries(static function1)
target_link_libraries(dynamic function1 function2)