#!/bin/bash
# set -x

GIT_DATE=$(git log -1 --format=%cd)

if [ ! -z "$GIT_DATE" ]
then
	export GIT_DATE
fi
