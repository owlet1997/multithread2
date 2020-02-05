cmake_minimum_required(VERSION 3.15)
project(multithread3)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")

add_executable(multithread3 main.cpp binarySort.cpp quickSort.cpp trapIntegra.cpp treeBinary.cpp)