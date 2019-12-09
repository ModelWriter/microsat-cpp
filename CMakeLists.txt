cmake_minimum_required(VERSION 3.15)
project(microsat++)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-o2 -g -Wall -Wpedantic -Wnull-dereference -Wnon-virtual-dtor)

#add_definitions(-DDEBUG)

add_executable(microsat++
        main.cpp
        tools.hpp
        tools.cpp
        solver.cpp
        solver.hpp
        driver.cpp
        driver.hpp
        memory.cpp
        memory.hpp)

add_executable(memory_allocation_test
        memory.cpp memory.hpp memory_test.cpp)