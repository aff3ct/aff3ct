#!/bin/bash
set -x

mkdir build
cd build

if [ -z "$CC" ]
then
	echo "Please define the 'CC' environment variable."
	exit 1
fi

if [ -z "$CXX" ]
then
	echo "Please define the 'CXX' environment variable."
	exit 1
fi

if [ -z "$THREADS" ]
then
	echo "The 'THREADS' environment variable is not set, default value = 1."
	THREADS=1
fi

if [ -z "$NAME" ]
then
	echo "The 'NAME' environment variable is not set, default value = 'build_linux_macos'."
	NAME="build_linux_macos"
fi

if [ -z "$LFLAGS" ]
then
	cmake .. -G"Unix Makefiles" -DCMAKE_CXX_COMPILER=$CC -DCMAKE_CXX_COMPILER=$CXX \
	         -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-Wall -funroll-loops -DENABLE_COOL_BASH $CFLAGS" \
	         -DENABLE_EXEC=ON -DENABLE_STATIC_LIB=ON -DENABLE_SHARED_LIB=ON $CMAKE_OPT
	rc=$?; if [[ $rc != 0 ]]; then exit $rc; fi
else
	cmake .. -G"Unix Makefiles" -DCMAKE_CXX_COMPILER=$CC -DCMAKE_CXX_COMPILER=$CXX \
	         -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-Wall -funroll-loops -DENABLE_COOL_BASH $CFLAGS" \
	         -DCMAKE_EXE_LINKER_FLAGS="$LFLAGS" \
	         -DENABLE_EXEC=ON -DENABLE_STATIC_LIB=ON -DENABLE_SHARED_LIB=ON $CMAKE_OPT
	rc=$?; if [[ $rc != 0 ]]; then exit $rc; fi
fi

make -j $THREADS -k
rc=$?; if [[ $rc != 0 ]]; then exit $rc; fi

mkdir $NAME $NAME/bin $NAME/lib $NAME/inc
cp bin/aff3ct $NAME/bin/aff3ct
cp lib/libaff3ct.a $NAME/lib/libaff3ct.a
cp -r ../src/* $NAME/inc/
find $NAME/inc/ -type f -follow -print | grep "[.]cpp$"    | xargs rm -f
find $NAME/inc/ -type f -follow -print | grep "[.]cpp.in$" | xargs rm -f

mv $NAME ../
