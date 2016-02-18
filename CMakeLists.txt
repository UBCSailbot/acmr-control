cmake_minimum_required(VERSION 3.4)
project(acmr_control)



set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)


add_executable(acmr_control ${SOURCE_FILES})


FIND_LIBRARY(ZMQ_LIB libzmq)
TARGET_LINK_LIBRARIES(acmr_control ${ZMQ_LIB})