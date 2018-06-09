#!/bin/bash
# set -x

HOSTNAME=$(hostname)

if [ $HOSTNAME = "vroum" ]
then
	THREADS=3
else
	THREADS=$(grep -c ^processor /proc/cpuinfo)
fi
export THREADS