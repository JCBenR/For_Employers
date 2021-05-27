cmake_minimum_required(VERSION 3.17)
project(MallocClone)

set(CMAKE_CXX_STANDARD 14)

add_executable(MallocClone main.cpp MyMalloc.cpp MyMalloc.h)