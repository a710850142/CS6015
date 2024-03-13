cmake_minimum_required(VERSION 3.27)
project(msd)

set(CMAKE_CXX_STANDARD 17)

add_executable(msd main.cpp
        cmdline.cpp
        exec.cpp
        expr.cpp
        parse.cpp
        Test.cpp
        test_msdscript.cpp
        val.cpp
        val.h)
