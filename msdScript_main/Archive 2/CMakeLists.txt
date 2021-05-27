cmake_minimum_required(VERSION 3.17)
project(MSDscript)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(MSDscript
        catch.h
        cmdline.cpp
        cmdline.h
        expr.cpp
        expr.h
        main.cpp
        Parse.cpp
        Parse.h
        Val.cpp
        Val.h
        Function.cpp
        Pointers.h Env.cpp Env.h Cont.cpp Cont.h Step.cpp Step.h)
