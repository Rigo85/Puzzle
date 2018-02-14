cmake_minimum_required(VERSION 3.9)
project(puzzlemaker)

set(CMAKE_CXX_STANDARD 17)

include_directories(. )

add_library(puzzlemaker SHARED PuzzleMaker.cpp)
