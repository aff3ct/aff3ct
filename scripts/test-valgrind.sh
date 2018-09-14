#!/bin/bash

if [ -z "$2" ]
then
	echo "Usage $0 build_path refs_path [result_file]"
	exit 1
fi

build_root=$1
refs_root=$2

log_file="valgrind_error.log"
if [ ! -z "$3" ]
then
	log_file="$3"
fi

touch ${log_file}

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

check_valgrind ${build_root} ${refs_root}

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
