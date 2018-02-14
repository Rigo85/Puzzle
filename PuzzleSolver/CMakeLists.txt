cmake_minimum_required(VERSION 3.9)
project(puzzlesolver)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -march=native -mtune=native")

include_directories(. )

add_library(puzzlesolver SHARED PuzzleSolver.cpp)
