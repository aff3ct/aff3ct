#!/bin/bash
set -x

THREADS=$(grep -c ^processor /proc/cpuinfo)

function compile {
	build=$1
	mkdir $build
	cd $build
	cmake .. -G"Unix Makefiles" -DCMAKE_CXX_COMPILER=g++ -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS_DEBUG="-g -O0" -DCMAKE_CXX_FLAGS="-Wall -funroll-loops -march=native -DMULTI_PREC -DENABLE_COOL_BASH --coverage" -DCMAKE_EXE_LINKER_FLAGS="--coverage"
	rc=$?; if [[ $rc != 0 ]]; then exit $rc; fi
	make -j $THREADS
	rc=$?; if [[ $rc != 0 ]]; then exit $rc; fi
	cd ..
}

build_root=build_coverage_linux_x86_gcc
compile "${build_root}"

function gen_coverage_info
{
	build=$1
	local folder=$2
	for path in $folder/*
	do [ -f $path ] && {
		cmd=$(cat $path | sed -n 2p)
		echo $cmd
		cd $build
		eval "${cmd} --sim-stop-time 1 -t 1"
		rc=$?; if [[ $rc != 0 ]]; then exit $rc; fi
		cd ..
	} || {
		[ -d $path ] && {
			gen_coverage_info $build $path
		}
 	}
	done
}

gen_coverage_info "${build_root}" "refs"

pwd=$(pwd)
mkdir code_coverage_files || true
lcov --capture --directory $build_root/CMakeFiles/aff3ct-obj.dir/src --output-file code_coverage_files/aff3ct.info
rc=$?; if [[ $rc != 0 ]]; then exit $rc; fi
lcov --remove code_coverage_files/aff3ct.info "*/usr*" "*lib/*" --output-file $pwd/code_coverage_files/aff3ct_clean.info
rc=$?; if [[ $rc != 0 ]]; then exit $rc; fi
mkdir code_coverage_report || true
genhtml code_coverage_files/aff3ct_clean.info --output-directory ./code_coverage_report/
