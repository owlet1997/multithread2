cmake_minimum_required(VERSION 3.15)
project(multithread7)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(multithread7 main.cpp secondPart.cpp secondPart.h )

