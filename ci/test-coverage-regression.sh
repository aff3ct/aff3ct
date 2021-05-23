#!/bin/bash
set -x

WD=$(pwd)
build_root=build

function gen_coverage_info
{
	build=$1
	local folder=$2
	for path in $folder/*
	do [ -f $path ] && {
		cmd=$(awk -F "=" '/command/ {print $2}' $path)
		cmd=$(echo "${cmd/aff3ct/.\/bin\/aff3ct-$GIT_VERSION}")
		echo $cmd
		ci=$(awk -F "=" '/ci/ {print $2}' $path)
		if [ "$ci" != "off" ]; then
			cd $build
			eval "${cmd} --sim-threads 1 --sim-max-fra 1 --sim-crit-nostop --ter-freq 0"
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

gen_coverage_info "${build_root}" "refs"
cd ${WD}

mkdir code_coverage_files || true
lcov --capture --directory $build_root/CMakeFiles/aff3ct-obj.dir/src --output-file code_coverage_files/aff3ct.info
rc=$?; if [[ $rc != 0 ]]; then exit $rc; fi
cp code_coverage_files/aff3ct.info code_coverage_files/aff3ct2.info
rc=$?; if [[ $rc != 0 ]]; then exit $rc; fi
sed -i -e "s#${WD}/#\./#g" code_coverage_files/aff3ct2.info
rc=$?; if [[ $rc != 0 ]]; then exit $rc; fi
lcov --remove code_coverage_files/aff3ct2.info "*/usr*" "*lib/*" "*/Tools/version.cpp" --output-file ${WD}/code_coverage_files/aff3ct_clean.info
