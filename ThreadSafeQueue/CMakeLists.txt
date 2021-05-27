cmake_minimum_required(VERSION 3.17)
project(ThreadSafeQueue)

set(CMAKE_CXX_STANDARD 14)
set( CMAKE_BUILD_TYPE Debug )

add_executable(ThreadSafeQueue
        SerialQueue.hpp
        SerialQueueTests.cpp
        ConcurrentQueue.h
        ConcurrentQueueTests.cpp
        main.cpp
        ConcurrentQueueTests.h
        SerialQueueTests.h)