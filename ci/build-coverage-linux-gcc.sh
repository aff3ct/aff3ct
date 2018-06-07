#!/bin/bash
set -x

THREADS=$(grep -c ^processor /proc/cpuinfo)

function compile {
	build=$1
	mkdir $build
	cd $build
	cmake .. -G"Unix Makefiles" -DCMAKE_CXX_COMPILER=g++ -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS_DEBUG="-g -O0" -DCMAKE_CXX_FLAGS="-Wall -funroll-loops -msse4.2 -DMULTI_PREC -DENABLE_COOL_BASH --coverage" -DCMAKE_EXE_LINKER_FLAGS="--coverage"
	rc=$?; if [[ $rc != 0 ]]; then exit $rc; fi
	make -j $THREADS
	rc=$?; if [[ $rc != 0 ]]; then exit $rc; fi
	cd ..
}

build_root=build_coverage_linux_x86_gcc
compile "${build_root}"
