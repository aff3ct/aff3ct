#!/bin/bash
set -x

THREADS=$(grep -c ^processor /proc/cpuinfo)
WD=$(pwd)
build_root=build_coverage_linux_x86_gcc

function compile {
	build=$1
	mkdir $build
	cd $build
	cmake .. -G"Unix Makefiles" -DCMAKE_CXX_COMPILER=g++ -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS_DEBUG="-O0" -DCMAKE_CXX_FLAGS="-Wall -funroll-loops -msse4.2 -DMULTI_PREC -DENABLE_COOL_BASH --coverage" -DCMAKE_EXE_LINKER_FLAGS="--coverage"
	rc=$?; if [[ $rc != 0 ]]; then exit $rc; fi
	make -j $THREADS
	rc=$?; if [[ $rc != 0 ]]; then exit $rc; fi
	cd ..
}

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

compile "${build_root}"
cd ${WD}
gen_coverage_info "${build_root}" "refs"
cd ${WD}

mkdir code_coverage_files || true
lcov --capture --directory $build_root/CMakeFiles/aff3ct-obj.dir/src --output-file code_coverage_files/aff3ct.info
rc=$?; if [[ $rc != 0 ]]; then exit $rc; fi
lcov --remove code_coverage_files/aff3ct.info "*/usr*" "*lib/*" "*/Tools/version.cpp" --output-file ${WD}/code_coverage_files/aff3ct_clean.info
rc=$?; if [[ $rc != 0 ]]; then exit $rc; fi
sed -i -e "s#${WD}/#\./#g" code_coverage_files/aff3ct_clean.info
