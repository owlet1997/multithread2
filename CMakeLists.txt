cmake_minimum_required(VERSION 3.15)
project(multithread4)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")

add_executable(multithread4 task2.cpp task2.h readFile.cpp readFile.h copyMatrix.cpp copyMatrix.h writer.cpp)