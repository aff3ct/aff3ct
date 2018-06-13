#!/bin/bash
# set -x

GIT_AUTHOR=$(git log -1 --pretty=format:'%an')

if [ -z "$GIT_AUTHOR" ]
then
	export GIT_AUTHOR
fi