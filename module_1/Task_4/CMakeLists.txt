cmake_minimum_required(VERSION 3.6)
project(Task_4)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp seminar.cpp)
add_executable(Task_4 ${SOURCE_FILES})