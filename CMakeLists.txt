cmake_minimum_required(VERSION 3.15)
project(multithread6)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")


add_executable(multithread6 main.cpp secondPart.cpp secondPart.h firstPart.cpp firstPart.h)