cmake_minimum_required(VERSION 3.10)
project(lw1)

find_library(PTHREAD_LIBRARY pthread)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(main main.cpp)
target_link_libraries(main ${PTHREAD_LIBRARY})
