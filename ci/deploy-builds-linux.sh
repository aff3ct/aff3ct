#!/bin/bash
set -x

if [[ ( -z "$CI_AFF3CT_DEPLOY" ) || ( "$CI_AFF3CT_DEPLOY" != "ON" ) ]]
then
	echo "This job is disabled, try to set the CI_AFF3CT_DEPLOY environment variable to 'ON' to enable it."
	exit 1
fi

if [ -z "$GIT_BRANCH" ]
then
	echo "Please define the 'GIT_BRANCH' environment variable."
	exit 1
fi

if [ -z "$GIT_TAG" ]
then
	echo "Please define the 'GIT_TAG' environment variable."
	exit 1
fi

if [ -z "$GIT_HASH" ]
then
	echo "Please define the 'GIT_HASH' environment variable."
	exit 1
fi

if [ -z "$GIT_DATE" ]
then
	echo "Please define the 'GIT_DATE' environment variable."
	exit 1
fi

if [ -z "$GIT_MESSAGE" ]
then
	echo "Please define the 'GIT_MESSAGE' environment variable."
	exit 1
fi

if [ -z "$GIT_AUTHOR" ]
then
	echo "Please define the 'GIT_AUTHOR' environment variable."
	exit 1
fi

if [ -z "$GIT_VERSION" ]
then
	echo "Please define the 'GIT_VERSION' environment variable."
	exit 1
fi

if [ -z "$CI_JOB_ID" ]
then
	echo "Please define the 'CI_JOB_ID' environment variable."
	exit 1
fi

if [ -z "$CI_PROJECT_URL" ]
then
	echo "Please define the 'CI_PROJECT_URL' environment variable."
	exit 1
fi

BUILDS_DIR=builds
mkdir ${BUILDS_DIR}
for BUILD in "$@"
do
	PREFIX=aff3ct_${GIT_BRANCH}
	ZIP_NAME=$(echo "${BUILD/build/$PREFIX}")
	ZIP_NAME=$(echo "${ZIP_NAME/\./\_}_$GIT_HASH.zip")

	zip -r $ZIP_NAME $BUILD > /dev/null
	rc=$?; if [[ $rc != 0 ]]; then exit $rc; fi

	mv $ZIP_NAME ${BUILDS_DIR}
	rm -rf $BUILD

	if [ -z "$BUILDS_LIST" ]
	then
		BUILDS_LIST="$ZIP_NAME"
	else
		BUILDS_LIST="$BUILDS_LIST;$ZIP_NAME"
	fi
done

REPO_WEB=aff3ct.github.io
git clone git@github.com:aff3ct/${REPO_WEB}.git
rc=$?; if [[ $rc != 0 ]]; then exit $rc; fi

cd ${REPO_WEB}

ARTIFACTS_PATH=${CI_PROJECT_URL}/-/jobs/${CI_JOB_ID}/artifacts/raw/file/${BUILDS_DIR}/
CSV_ENTRY=$(cat download/download_${GIT_BRANCH}.csv | grep "$GIT_TAG" | grep "$GIT_HASH" | grep "$GIT_DATE" | grep "$GIT_MESSAGE" | grep "$GIT_AUTHOR" | grep "$ARTIFACTS_PATH" | grep "$BUILDS_LIST")

# if the entry does not exist, add the entry to the CSV file
if [ -z "$CSV_ENTRY" ]; then
	echo "\"$GIT_TAG\",\"$GIT_HASH\",\"$GIT_DATE\",\"$GIT_MESSAGE\",\"$GIT_AUTHOR\",\"$ARTIFACTS_PATH\",\"$BUILDS_LIST\"" >> download/download_${GIT_BRANCH}.csv
	git add -f download/download_${GIT_BRANCH}.csv
	git commit -m "Automatic: add new AFF3CT builds ($GIT_BRANCH: $GIT_HASH)."
	git push origin master
	rc=$?; if [[ $rc != 0 ]]; then exit $rc; fi
fi