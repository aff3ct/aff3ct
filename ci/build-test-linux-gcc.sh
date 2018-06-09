#!/bin/bash
set -x

source ci/threads.sh

build_root=build_test_linux_x86_gcc

mkdir ${build_root}
cd ${build_root}
cmake .. -G"Unix Makefiles" -DCMAKE_CXX_COMPILER=g++ -DCMAKE_BUILD_TYPE=Release -DENABLE_GSL=ON -DCMAKE_CXX_FLAGS="-g -Wall -funroll-loops -msse4.2 -DMULTI_PREC -DENABLE_COOL_BASH"
make -j $THREADS