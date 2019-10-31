#!/bin/bash
set -x

if [[ ( -z "$CI_AFF3CT_DEPLOY" ) || ( "$CI_AFF3CT_DEPLOY" != "ON" ) ]]
then
	echo "This job is disabled, try to set the CI_AFF3CT_DEPLOY environment variable to 'ON' to enable it."
	exit 1
fi

cmake --version
mkdir build
mv doc/build doc/built
cd build

if [ -z "DISTRIBS" ]
then
	echo "Please define the 'DISTRIBS' environment variable."
	exit 1
fi


if [ -z "$CXX" ]
then
	echo "Please define the 'CXX' environment variable."
	exit 1
fi

if [ -z "$GIT_BRANCH" ]
then
	echo "Please define the 'GIT_BRANCH' environment variable."
	exit 1
fi

if [ -z "$THREADS" ]
then
	echo "The 'THREADS' environment variable is not set, default value = 1."
	THREADS=1
fi

if [ -z "$NAME" ]
then
	echo "The 'NAME' environment variable is not set, default value = 'build_deploy_upload_ppa'."
	NAME="build_deploy_upload_ppa"
fi

if [ "$GIT_BRANCH" = "development" ]
then
	DPUT_HOST="ppa:aff3ct/aff3ct-dev"
elif [ "$GIT_BRANCH" = "master" ]
then
	DPUT_HOST="ppa:aff3ct/aff3ct-stable"
elif [ "$GIT_BRANCH" = "ppa_upload" ]
then
	DPUT_HOST="ppa:aff3ct/aff3ct-dev"
else
	echo "Deploy upload ppa must be run only on master or development branch."
	exit 1
fi

if [ -z "$LFLAGS" ]
then
	cmake .. -G"Unix Makefiles" -DCMAKE_CXX_COMPILER=$CXX \
	         -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="$CFLAGS" \
	         $CMAKE_OPT -DCMAKE_INSTALL_PREFIX="$NAME" \
	         -DAFF3CT_UPLOAD_PPA="ON" -DAFF3CT_DPUT_HOST="$DPUT_HOST" \
	         -DAFF3CT_PPA_DISTRIB="$DISTRIBS"
	rc=$?; if [[ $rc != 0 ]]; then exit $rc; fi
else
	cmake .. -G"Unix Makefiles" -DCMAKE_CXX_COMPILER=$CXX \
	         -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="$CFLAGS" \
	         -DCMAKE_EXE_LINKER_FLAGS="$LFLAGS" \
	         $CMAKE_OPT -DCMAKE_INSTALL_PREFIX="$NAME" \
	         -DAFF3CT_UPLOAD_PPA="ON" -DAFF3CT_DPUT_HOST="$DPUT_HOST" \
	         -DAFF3CT_PPA_DISTRIB="$DISTRIBS"
	rc=$?; if [[ $rc != 0 ]]; then exit $rc; fi
fi

make -j $THREADS -k
rc=$?; if [[ $rc != 0 ]]; then exit $rc; fi
