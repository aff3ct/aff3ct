#!/bin/bash
set -x

git clone git@github.com:aff3ct/aff3ct.github.io.git
mkdir aff3ct.github.io/ressources/builds

for BUILD in "$@"
do
	PREFIX=aff3ct_${GIT_BRANCH}
	ZIP_NAME=$(echo "${BUILD/build/$PREFIX}")
	ZIP_NAME=$(echo "${ZIP_NAME/\./\_}_$GIT_HASH.zip")

	find $BUILD/inc/ -type f -follow -print | grep "[.]cpp$"    | xargs rm -f
	find $BUILD/inc/ -type f -follow -print | grep "[.]cpp.in$" | xargs rm -f
	cp -r conf $BUILD/
	rm -rf $BUILD/conf/.git

	zip -r $ZIP_NAME $BUILD

	cp $ZIP_NAME aff3ct.github.io/ressources/builds/
done

echo "\"$GIT_TAG\";\"$GIT_HASH\";\"$GIT_DATE\";\"$GIT_MESSAGE\";\"$GIT_AUTHOR\"" >> aff3ct.github.io/ressources/download_${GIT_BRANCH}.csv

cd aff3ct.github.io
git add -f ressources/builds/*
git add -f ressources/download_${GIT_BRANCH}.csv
git commit -m "Automatic from Gitlab: add new AFF3CT builds to the download section ($GIT_HASH)."
git push origin master