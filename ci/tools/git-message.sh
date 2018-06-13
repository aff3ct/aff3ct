#!/bin/bash
# set -x

GIT_MESSAGE=$(git log -1 --pretty=%B | head -n 1)

if [ -z "$GIT_MESSAGE" ]
then
	export GIT_MESSAGE
fi
