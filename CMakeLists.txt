cmake_minimum_required(VERSION 3.15)
project(microsat++)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-O1 -g -Wall -Wextra -Wpedantic -Wnull-dereference -Wnon-virtual-dtor)

#add_definitions(-DDEBUG)

add_executable(microsat++
        main.cpp
        tools.hpp
        tools.cpp solver.cpp solver.hpp driver.cpp driver.hpp)