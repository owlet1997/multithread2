cmake_minimum_required(VERSION 3.15)
project(multithread1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")

add_executable(multithread1 main.cpp)