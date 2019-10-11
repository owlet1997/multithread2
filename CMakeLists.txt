cmake_minimum_required(VERSION 3.13)
project(multithread2)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")
set_target_properties(${multithread2} PROPERTIES LINK_FLAGS "${OpenMP_CXX_FLAGS}")

add_executable(multithread2 main2_1.cpp main2_2.cpp task1.cpp task1.h task2.cpp task2.h task3.cpp task3.h)