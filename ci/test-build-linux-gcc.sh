#!/bin/bash
set -x

mkdir build
cd build
cmake .. -G"Unix Makefiles" -DCMAKE_CXX_COMPILER=g++ -DCMAKE_BUILD_TYPE=Release -DENABLE_GSL=ON -DCMAKE_CXX_FLAGS="-g -Wall -funroll-loops -march=native -DMULTI_PREC -DENABLE_COOL_BASH"
THREADS=$(grep -c ^processor /proc/cpuinfo)
make -j $THREADS