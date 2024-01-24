cmake_minimum_required(VERSION 3.27)
project(cs6015_hw2)

set(CMAKE_CXX_STANDARD 17)

add_executable(cs6015_hw2 main.cpp
        cmdline.cpp
        expr.cpp
        Test.cpp)
