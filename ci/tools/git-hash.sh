#!/bin/bash
# set -x

GIT_HASH=$(git rev-parse --short HEAD)

if [ -z "$GIT_HASH" ]
then
	export GIT_HASH
fi
