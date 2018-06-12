#!/bin/bash
# set -x

if [ -z "$THREADS" ]; then
	if [ ! -f /proc/cpuinfo ]; then
		export THREADS=1
	else
		THREADS=$(grep -c ^processor /proc/cpuinfo)
		export THREADS
	fi
fi
