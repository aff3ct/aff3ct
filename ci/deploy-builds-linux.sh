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

REPO_WEB=aff3ct.github.io
REPO_RESSOURCES=ressources
git clone git@github.com:aff3ct/${REPO_WEB}.git
rc=$?; if [[ $rc != 0 ]]; then exit $rc; fi
cd ${REPO_WEB}
git clone git@github.com:aff3ct/${REPO_RESSOURCES}.git
rc=$?; if [[ $rc != 0 ]]; then exit $rc; fi
git submodule update --init --recursive
mkdir ressources/aff3ct_builds
cd ..

CSV_ENTRY=$(cat ${REPO_WEB}/download/download_${GIT_BRANCH}.csv | grep "$GIT_TAG" | grep "$GIT_HASH" | grep "$GIT_DATE" | grep "$GIT_MESSAGE" | grep "$GIT_AUTHOR" | grep "$BUILDS_LIST")

# delete the build if it exists
if [ -n "$CSV_ENTRY" ]; then
	cd ${REPO_WEB}/${REPO_RESSOURCES}
	FILES=$(echo $CSV_ENTRY | cut -d ";" -f6)
	s1="\""
	s2=""
	FILES=$(echo "${FILES//$s1/$s2}")
	N_FILES=$(echo $FILES |  sed 's/,/\n/g' | wc -l)
	for (( F=1; F<=N_FILES; F++ ))
	do
		FILE=$(echo $FILES | cut -d "," -f$F)
		FILE_PATH=aff3ct_builds/$FILE;
		if [ -f $FILE_PATH ]; then
			git filter-branch --force --index-filter "git rm --cached --ignore-unmatch ${FILE_PATH}" --prune-empty --tag-name-filter cat -- --all
			rm -rf .git/refs/original/
			git reflog expire --expire=now --all
		fi
	done
	cd ../../
fi

for BUILD in "$@"
do
	PREFIX=aff3ct_${GIT_BRANCH}
	ZIP_NAME=$(echo "${BUILD/build/$PREFIX}")
	ZIP_NAME=$(echo "${ZIP_NAME/\./\_}_$GIT_HASH.zip")

	# prepare headers
	find $BUILD/include/aff3ct/ -type f -follow -print | grep "[.]cpp$"    | xargs rm -f
	find $BUILD/include/aff3ct/ -type f -follow -print | grep "[.]cpp.in$" | xargs rm -f
	cp -r lib/date/include/date $BUILD/include/
	mkdir $BUILD/include/MIPP
	cp -r lib/MIPP/src/* $BUILD/include/MIPP/
	mkdir $BUILD/include/dirent
	cp lib/MSVC/include/dirent.h $BUILD/include/dirent/
	mkdir $BUILD/include/rang
	cp lib/rang/include/rang.hpp $BUILD/include/rang/

	mv $BUILD/include $BUILD/aff3ct-$GIT_VERSION
	mkdir $BUILD/include
	mv $BUILD/aff3ct-$GIT_VERSION $BUILD/include/

	# prepare conf and refs files
	mkdir $BUILD/share
	mkdir $BUILD/share/aff3ct-$GIT_VERSION
	cp -r conf $BUILD/share/aff3ct-$GIT_VERSION/
	cp -r refs $BUILD/share/aff3ct-$GIT_VERSION/
	rm -rf $BUILD/share/aff3ct-$GIT_VERSION/conf/.git
	rm -rf $BUILD/share/aff3ct-$GIT_VERSION/refs/.git
	rm -rf $BUILD/share/aff3ct-$GIT_VERSION/refs/readers
	mkdir $BUILD/share/aff3ct-$GIT_VERSION/doc
	mkdir $BUILD/share/aff3ct-$GIT_VERSION/doc/strings
	cp doc/sphinx/strings.rst $BUILD/share/aff3ct-$GIT_VERSION/doc/strings

	# prepare doc
	mkdir $BUILD/share/aff3ct-$GIT_VERSION/doc
	cp -r doc/sphinx/build/html $BUILD/share/aff3ct-$GIT_VERSION/doc/
	mkdir $BUILD/share/aff3ct-$GIT_VERSION/doc/pdf
	cp doc/sphinx/build/latex/AFF3CT.pdf $BUILD/share/aff3ct-$GIT_VERSION/doc/pdf

	zip -r $ZIP_NAME $BUILD
	rc=$?; if [[ $rc != 0 ]]; then exit $rc; fi

	mv $ZIP_NAME ${REPO_WEB}/${REPO_RESSOURCES}/aff3ct_builds/
	rm -rf $BUILD

	if [ -z "$BUILDS_LIST" ]
	then
		BUILDS_LIST=${ZIP_NAME}
	else
		BUILDS_LIST=${BUILDS_LIST},${ZIP_NAME}
	fi
done

# if the entry does not exist, add the entry to the CSV file
if [ -z "$CSV_ENTRY" ]; then
	echo "\"$GIT_TAG\";\"$GIT_HASH\";\"$GIT_DATE\";\"$GIT_MESSAGE\";\"$GIT_AUTHOR\";\"$BUILDS_LIST\"" >> ${REPO_WEB}/download/download_${GIT_BRANCH}.csv
fi

cd ${REPO_WEB}/${REPO_RESSOURCES}
# git lfs install --local
# git lfs track aff3ct_builds/*
git add -f aff3ct_builds/*
git commit -m "Automatic: add new AFF3CT builds ($GIT_BRANCH: $GIT_HASH)."

# delete old builds
BUILD_CSV=../download/download_${GIT_BRANCH}.csv
N_BUILDS_TO_KEEP=0
if [ "${GIT_BRANCH}" == "master" ]; then
	N_BUILDS_TO_KEEP=5
else
	N_BUILDS_TO_KEEP=5
fi
N_BUILDS=$(wc -l $BUILD_CSV | cut -d " " -f1)
N_BUILDS=$(($N_BUILDS-1))
N_BUILDS_TO_RM=$(($N_BUILDS-$N_BUILDS_TO_KEEP))
if (( $N_BUILDS_TO_RM >= 1 )); then
	B=0
	cat $BUILD_CSV | while read line
	do
		if (( $B != 0 )); then
			FILES=$(echo $line | cut -d ";" -f6)
			s1="\""
			s2=""
			FILES=$(echo "${FILES//$s1/$s2}")
			N_FILES=$(echo $FILES |  sed 's/,/\n/g' | wc -l)
			for (( F=1; F<=N_FILES; F++ ))
			do
				FILE=$(echo $FILES | cut -d "," -f$F)
				FILE_PATH=aff3ct_builds/$FILE;
				if [ -f $FILE_PATH ]; then
					git filter-branch --force --index-filter "git rm --cached --ignore-unmatch ${FILE_PATH}" --prune-empty --tag-name-filter cat -- --all
					rm -rf .git/refs/original/
					git reflog expire --expire=now --all
				fi
			done
		fi
		if [ "${B}" -eq $N_BUILDS_TO_RM ]; then
			break
		fi
		B=$((B+1))
	done
fi

git gc --prune=now
# git gc --aggressive --prune=now
git push origin master --force
rc=$?; if [[ $rc != 0 ]]; then exit $rc; fi

cd ..
git add -f download/download_${GIT_BRANCH}.csv
git commit -m "Automatic: add new AFF3CT builds ($GIT_BRANCH: $GIT_HASH)."
git push origin master
rc=$?; if [[ $rc != 0 ]]; then exit $rc; fi