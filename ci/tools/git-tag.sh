#!/bin/bash
# set -x

GIT_TAG=$(git tag | tail -n 1)

if [ ! -z "$GIT_TAG" ]
then
	export GIT_TAG
fi
