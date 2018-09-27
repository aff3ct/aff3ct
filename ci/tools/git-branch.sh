#!/bin/bash
# set -x

# GIT_BRANCH=$(git show -s --pretty=%d HEAD | sed -rne 's/.*origin\/(.*),.*/\1/p')
# if [ -z "$GIT_BRANCH" ]
# then
# 	GIT_BRANCH=$(git show -s --pretty=%d HEAD | sed -rne 's/.*origin\/(.*)\)$/\1/p')
# fi

# if [ -z "$GIT_BRANCH" ]
# then
# 	GIT_BRANCH=$(git branch --remote --verbose --no-abbrev --contains | sed -rne 's/^[^\/]*\/([^\ ]+).*$/\1/p')
# fi

# if [ ! -z "$GIT_BRANCH" ]
# then
# 	export GIT_BRANCH
# fi

export GIT_BRANCH=development
