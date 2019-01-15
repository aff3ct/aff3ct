#!/bin/bash
set -x

if [ -z "$GIT_BRANCH" ]
then
	echo "Please define the 'GIT_BRANCH' environment variable."
	exit 1
fi

cd conf
HASH_CONF=$(git rev-parse --short HEAD)
git checkout ${GIT_BRANCH}
git pull origin ${GIT_BRANCH}
CONTAIN_COMMIT_CONF=$(git branch ${GIT_BRANCH} --contains ${HASH_CONF})
if [ -z "$CONTAIN_COMMIT_CONF" ]
then
	echo "The '$HASH_CONF' commit of the 'conf' submodule does not exist in the '${GIT_BRANCH}' branch."
	exit 1
fi
cd ..

cd refs
HASH_REFS=$(git rev-parse --short HEAD)
git checkout ${GIT_BRANCH}
git pull origin ${GIT_BRANCH}
CONTAIN_COMMIT_REFS=$(git branch ${GIT_BRANCH} --contains ${HASH_REFS})
if [ -z "$CONTAIN_COMMIT_REFS" ]
then
	echo "The '$HASH_CONF' commit of the 'refs' submodule does not exist in the '${GIT_BRANCH}' branch."
	exit 1
fi
cd ..

exit 0
