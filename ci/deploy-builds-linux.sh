#!/bin/bash
set -x

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

REPO_WEB=aff3ct.github.io
REPO_RESSOURCES=ressources
git clone git@github.com:aff3ct/${REPO_WEB}.git
cd ${REPO_WEB}
git clone git@github.com:aff3ct/${REPO_RESSOURCES}.git
git submodule update --init --recursive
mkdir ressources/aff3ct_builds
cd ..

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

	cp $ZIP_NAME ${REPO_WEB}/${REPO_RESSOURCES}/aff3ct_builds/

	if [ -z "$BUILDS_LIST" ]
	then
		BUILDS_LIST=${ZIP_NAME}
	else
		BUILDS_LIST=${BUILDS_LIST},${ZIP_NAME}
	fi
done

echo "\"$GIT_TAG\";\"$GIT_HASH\";\"$GIT_DATE\";\"$GIT_MESSAGE\";\"$GIT_AUTHOR\";\"$BUILDS_LIST\"" >> ${REPO_WEB}/download/download_${GIT_BRANCH}.csv

cd ${REPO_WEB}/${REPO_RESSOURCES}
# git lfs install --local
# git lfs track aff3ct_builds/*
git add -f aff3ct_builds/*
git commit -m "Automatic: add new AFF3CT builds ($GIT_HASH)."

#delete old builds
BUILD_CSV=../download/download_${GIT_BRANCH}.csv
N_BUILDS_TO_KEEP=0
if [ "${GIT_BRANCH}" == "master" ]; then
	N_BUILDS_TO_KEEP=5
else
	N_BUILDS_TO_KEEP=10
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

cd ..
git add -f download/download_${GIT_BRANCH}.csv
git commit -m "Automatic: add new AFF3CT builds ($GIT_HASH)."
git push origin master