cmake_minimum_required(VERSION 3.10)
project(lw3)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(main main.cpp)
add_executable(child_process child_process.cpp)