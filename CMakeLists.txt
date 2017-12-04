cmake_minimum_required(VERSION 3.8)
project(Recognition)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp MatrixClass.cpp Recognition.cpp Recognition.h)
add_executable(Recognition ${SOURCE_FILES})