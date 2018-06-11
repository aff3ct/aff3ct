#!/bin/bash
# set -x

if [ -z "$THREADS" ]; then
	THREADS=$(grep -c ^processor /proc/cpuinfo)
	export THREADS
fi
