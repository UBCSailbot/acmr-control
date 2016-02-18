cmake_minimum_required(VERSION 3.3)
project(acmr_control)



set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES ZMQHandler.cpp ZMQHandler.h PID.cpp PID.h)

find_package(PkgConfig REQUIRED)
pkg_search_module(ZMQ REQUIRED libzmq)
include_directories(${ZMQ_INCLUDE_DIRS})
link_directories(${ZMQ_LIBRARY_DIRS})

add_executable(acmr_tcu TCU.cpp ${SOURCE_FILES})
add_executable(acmr_scu SCU.cpp ${SOURCE_FILES} schemes.h)


FIND_LIBRARY(ZMQ_LIB libzmq)
TARGET_LINK_LIBRARIES(acmr_tcu ${ZMQ_LIBRARIES})
TARGET_LINK_LIBRARIES(acmr_scu ${ZMQ_LIBRARIES})

