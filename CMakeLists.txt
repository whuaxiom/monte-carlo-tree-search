cmake_minimum_required(VERSION 3.9)
project(mcts)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-g -Wall -O3 -Wno-sign-compare -fpermissive")

include_directories(mcts/include)
include_directories(tiny-dnn)

add_executable(mcts mcts/src/main.cpp)
add_executable(example_cifar10_train test/train.cpp)
