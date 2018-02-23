#!/bin/sh -x

cur_dir=`pwd`
deps_dir=${cur_dir}/deps
packages_dir=${cur_dir}/packages
build_dir=${cur_dir}/build

rm -rf ${build_dir}
mkdir ${build_dir}

cd ${build_dir}/
tar -zxvf ${packages_dir}/protobuf-cpp-3.4.1.tar.gz

cd ${cur_dir}

rm -rf ${deps_dir}
mkdir -p ${deps_dir}/include/
mkdir -p ${deps_dir}/lib

# protobuf
cd ${build_dir}/protobuf-3.4.1
export CFLAGS=-fPIC && export CXXFLAGS=-fPIC && ./autogen.sh && ./configure -prefix=${deps_dir} && make -j32 && make install

cd ${cur_dir}

