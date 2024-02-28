cmake_minimum_required(VERSION 3.27)
project(untitled)

set(CMAKE_CXX_STANDARD 17)

add_executable(untitled main.cpp
        cmdline.cpp
        cmdline.h
        expr.cpp
        expr.h
        Test.cpp
        parse.cpp
        parse.h
        parse-test.cpp
        exec.h
        exec.cpp
        test_msdscript.cpp
        run_msdscript.cpp
        run_msdscript2.cpp
        run_msdscript2.cpp
        run_msdscript.cpp
        test_msdscript.cpp)
