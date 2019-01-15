#!/bin/bash
set -x

if [ -z "$GIT_BRANCH" ]
then
	echo "Please define the 'GIT_BRANCH' environment variable."
	exit 1
fi

GITHUB=git@github.com:aff3ct

REPO_REFS=error_rate_references
cd refs
git remote add github ${GITHUB}/${REPO_REFS}.git
git checkout ${GIT_BRANCH}
git pull origin ${GIT_BRANCH}
git push github ${GIT_BRANCH}
rc=$?; if [[ $rc != 0 ]]; then exit $rc; fi
cd ../

REPO_CONF=configuration_files
cd conf
git remote add github ${GITHUB}/${REPO_CONF}.git
git checkout ${GIT_BRANCH}
git pull origin ${GIT_BRANCH}
git push github ${GIT_BRANCH}
rc=$?; if [[ $rc != 0 ]]; then exit $rc; fi
cd ../

REPO_AFF3CT=aff3ct
git remote add github ${GITHUB}/${REPO_AFF3CT}.git
git checkout ${GIT_BRANCH}
git pull origin ${GIT_BRANCH}
git push github ${GIT_BRANCH}
rc=$?; if [[ $rc != 0 ]]; then exit $rc; fi