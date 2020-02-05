cmake_minimum_required(VERSION 3.15)
project(multithread5)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")

add_executable(multithread5 main.cpp diningPhilosophers.cpp diningPhilosophers.h writersReaders.cpp writersReaders.h)