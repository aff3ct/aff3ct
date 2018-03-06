#!/bin/bash
set -x

cp $SYSTEMC_HOME/FindSystemC.cmake cmake/Modules/
cp $SYSTEMC_HOME/FindTLM.cmake cmake/Modules/

mkdir build
cd build
cmake .. -G"Unix Makefiles" -DCMAKE_CXX_COMPILER=g++ -DCMAKE_BUILD_TYPE=Release -DENABLE_SYSTEMC=ON -DCMAKE_CXX_FLAGS="-Wall -Wno-deprecated-declarations -funroll-loops -march=native -DMULTI_PREC"
THREADS=$(grep -c ^processor /proc/cpuinfo)
make -j $THREADS

./bin/aff3ct --version
./bin/aff3ct -t 1 -C POLAR -m 1.0 -M 3.5 -s 0.25 -K 1755 -N 2048 --crc-poly 32-GZIP --dec-type ASCL --ter-freq 0