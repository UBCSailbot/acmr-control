cmake_minimum_required(VERSION 3.3)
project(acmr_control)



set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp ZMQHandler.cpp ZMQHandler.h)

find_package(PkgConfig REQUIRED)
pkg_search_module(ZMQ REQUIRED libzmq)
include_directories(${ZMQ_INCLUDE_DIRS})
link_directories(${ZMQ_LIBRARY_DIRS})

add_executable(acmr_control ${SOURCE_FILES})


FIND_LIBRARY(ZMQ_LIB libzmq)
TARGET_LINK_LIBRARIES(acmr_control ${ZMQ_LIBRARIES})

