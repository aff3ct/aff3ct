#!/bin/bash
set -x

source ci/threads.sh

source /opt/intel/vars-intel.sh
mkdir build
cd build
#cmake .. -G"Unix Makefiles" -DCMAKE_CXX_COMPILER=icpc -DCMAKE_BUILD_TYPE=Release -DENABLE_GSL=ON -DENABLE_MKL=ON -DCMAKE_CXX_FLAGS="-funroll-loops -march=native -DMULTI_PREC -std=c++11 -Wall -Warray-bounds -Wchar-subscripts -Wcomment -Wenum-compare -Wformat -Wuninitialized -Wmaybe-uninitialized -Wmain -Wnarrowing -Wnonnull -Wparentheses -Wpointer-sign -Wreorder -Wreturn-type -Wsign-compare -Wsequence-point -Wtrigraphs -Wunused-function -Wunused-but-set-variable -Wunused-variable -Wwrite-strings"
cmake .. -G"Unix Makefiles" -DCMAKE_CXX_COMPILER=icpc -DCMAKE_BUILD_TYPE=Release -DENABLE_GSL=ON -DENABLE_MKL=ON -DCMAKE_CXX_FLAGS="-funroll-loops -march=native -DMULTI_PREC -std=c++11 -Wall"
make -j $THREADS

./bin/aff3ct --version
./bin/aff3ct -C POLAR -m 1.0 -M 3.5 -s 0.25 -K 1755 -N 2048 --chn-implem MKL --crc-poly 32-GZIP --dec-type ASCL --ter-freq 0