cmake_minimum_required(VERSION 3.9)
project(mcts)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-g -Wall -O3 -Wno-sign-compare -fpermissive")

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(mcts src/main.cpp)
