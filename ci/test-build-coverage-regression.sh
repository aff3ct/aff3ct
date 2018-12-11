#!/bin/bash
set -x

if [ -z "$THREADS" ]
then
	echo "The 'THREADS' environment variable is not set, default value = 1."
	THREADS=1
fi

WD=$(pwd)
build_root=build_coverage_linux_x86_gcc

function compile {
	build=$1
	mkdir $build
	cd $build
	cmake .. -G"Unix Makefiles" -DCMAKE_CXX_COMPILER=g++ -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS_DEBUG="-O0" -DCMAKE_CXX_FLAGS="-Wall -funroll-loops -msse4.2 --coverage" -DCMAKE_EXE_LINKER_FLAGS="--coverage" -DAFF3CT_COMPILE_EXE="ON" -DAFF3CT_PREC="MULTI"
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
		cmd=$(awk -F "=" '/command/ {print $2}' $path)
		cmd=$(echo "${cmd/aff3ct/aff3ct-$GIT_VERSION}")
		echo $cmd
		ci=$(awk -F "=" '/ci/ {print $2}' $path)
		if [ "$ci" != "off" ]; then
			cd $build
			eval "${cmd} --sim-stop-time 1 -t 1"
			rc=$?; if [[ $rc != 0 ]]; then exit $rc; fi
			cd ..
		else
			echo "The code coverage is skipped because this test is disabled in the CI."
		fi
	} || {
		[ -d $path ] && {
			if [[ ${path: -7} == "readers" || ${path: -11} == "__pycache__" ]]; then
				continue
			fi
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
