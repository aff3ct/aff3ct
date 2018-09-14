#!/bin/bash
set -x

if [ -z "$THREADS" ]
then
	echo "The 'THREADS' environment variable is not set, default value = 1."
	THREADS=1
fi

WD=$(pwd)
build_root=build_coverage_linux_x86_gcc

output_folder="valgrind";
mkdir -p ${output_folder}

log_file="${output_folder}/valgrind_error.log";
touch ${log_file}

function compile {
	build=$1
	mkdir $build
	cd $build
	cmake .. -G"Unix Makefiles" -DCMAKE_CXX_COMPILER=g++ -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS_DEBUG="-O0" -DCMAKE_CXX_FLAGS="-Wall -funroll-loops -msse4.2 -DMULTI_PREC -DENABLE_COOL_BASH" -DCMAKE_EXE_LINKER_FLAGS=""
	rc=$?; if [[ $rc != 0 ]]; then exit $rc; fi
	make -j $THREADS
	rc=$?; if [[ $rc != 0 ]]; then exit $rc; fi
	cd ..
}

function check_valgrind
{
	build=$1
	local folder=$2
	for path in $folder/*
	do [ -f $path ] && {
		cmd=$(awk -F "=" '/command/ {print $2}' $path)
		echo $cmd
		ci=$(awk -F "=" '/ci/ {print $2}' $path)
		if [ "$ci" != "off" ]; then
			cd $build
			eval "valgrind -q --error-exitcode=1 --leak-check=full ${cmd} --sim-stop-time 1 -t 1 -e 10000"
			rc=$?;
			if [[ $rc != 0 ]]; then
				echo "MEMCHECK ERROR";
				echo "valgrind --leak-check=full ${cmd}" >> ${log_file}
			fi
			cd ..
		else
			echo "The valgrind check is skipped because this test is disabled in the CI."
		fi
	} || {
		[ -d $path ] && {
			if [[ ${path: -7} == "readers" || ${path: -11} == "__pycache__" ]]; then
				continue
			fi
			check_valgrind $build $path
		}
	}
	done
}

compile "${build_root}"
cd ${WD}
check_valgrind "${build_root}" "refs"

COUNT=$(wc -l < ${log_file} )

if [ $COUNT -gt 0 ]; then
	echo "Error count is $COUNT! Memory check run failed :-(.";
	echo "Search for 'MEMCHECK ERROR'."
	echo ""
	echo "Errors list:"
	cat ${log_file}
	exit 1;
else
	echo "There is no error :-)."
fi

exit 0;
