cmake_minimum_required(VERSION 3.9)

# set(CMAKE_C_COMPILER "/usr/local/bin/gcc")
# set(CMAKE_CXX_COMPILER "/usr/local/bin/g++")

project(mcts)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-g -Wall -O3 -Wno-sign-compare -fpermissive")

# set(DEPS_DIR /usr/local)
set(DEPS_DIR ${CMAKE_SOURCE_DIR}/third/deps)
set(PROTO_DIR ${CMAKE_SOURCE_DIR}/proto)

file(GLOB PROTO_FILES "${PROTO_DIR}/*.proto")
execute_process(COMMAND ${DEPS_DIR}/bin/protoc --cpp_out=${PROTO_DIR} --proto_path=${PROTO_DIR} ${PROTO_FILES})
execute_process(COMMAND ${DEPS_DIR}/bin/protoc --python_out=${PROTO_DIR} --proto_path=${PROTO_DIR} ${PROTO_FILES})

include_directories(${DEPS_DIR}/include)
include_directories(${PROTO_DIR})
include_directories(${CMAKE_SOURCE_DIR}/mcts/include)

find_library(PROTOBUF libprotobuf.a ${DEPS_DIR}/lib)
set(MODULE_LINKER_FLAGS ${PROTOBUF})
set(LINKER_FLAGS "-v")
# set(LINKER_FLAGS "-Wl,-static -static-libgcc -static-libstdc++ -v")

file(GLOB SRC_FILES "${PROTO_CPP_DIR}/*.cc" "mcts/src/*.cc")

add_executable(search ${CMAKE_SOURCE_DIR}/mcts/src/main.cc)
add_library(mcts ${SRC_FILES})
# target_link_libraries(search mcts ${LINKER_FLAGS} ${MODULE_LINKER_FLAGS})
target_link_libraries(search mcts ${MODULE_LINKER_FLAGS})
