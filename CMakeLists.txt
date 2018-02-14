cmake_minimum_required(VERSION 3.9)

project(Puzzle)

add_subdirectory(PuzzleMaker)
add_subdirectory(PuzzleSolver)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++17 -Wall -march=native -mtune=native")
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake-extensions/)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
set(SOURCE_FILES main.cpp GUI/mvc/view/PuzzleSolverWindow.cpp GUI/mvc/model/Worker.cpp GUI/mvc/model/Notification.cpp
        GUI/mvc/di/Utils.cpp GUI/mvc/di/PuzzleMakerImplementation.cpp)

find_package(PkgConfig)

pkg_check_modules(GTKMM gtkmm-3.0)
pkg_check_modules(OPENCV opencv)

link_directories(${GTKMM_LIBRARY_DIRS} ${OPENCV_LIBRARY_DIRS})

include_directories(${GTKMM_INCLUDE_DIRS} ${OPENCV_INCLUDE_DIRS} GUI/mvc/view GUI/mvc/model GUI/mvc/di)

add_executable(Puzzle ${SOURCE_FILES})

target_link_libraries(Puzzle puzzlemaker puzzlesolver ${GTKMM_LIBRARIES} pthread  ${OPENCV_LIBRARIES} stdc++fs)
