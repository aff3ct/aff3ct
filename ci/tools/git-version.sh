#!/bin/bash
# set -x

GIT_VERSION=$(git describe)

if [ ! -z "$GIT_VERSION" ]
then
	GIT_VERSION=$(echo $GIT_VERSION | cut -d $'v' -f2-)
	export GIT_VERSION
fi
