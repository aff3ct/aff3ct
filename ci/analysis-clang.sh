#!/bin/bash
set -x

if [ -z "$THREADS" ]
then
	echo "The 'THREADS' environment variable is not set, default value = 1."
	THREADS=1
fi

if [ -z "$NAME" ]
then
	echo "The 'NAME' environment variable is not set, default value = 'build_analysis_clang'."
	NAME="build_analysis_clang"
fi

if [ -z "$REPORTS_DIR" ]
then
	echo "The 'REPORTS_DIR' environment variable is not set, default value = 'clang_reports'."
	REPORTS_DIR="clang_reports"
fi

mkdir -p $NAME
cd $NAME

scan-build -v -plist --intercept-first --analyze-headers -o $REPORTS_DIR \
           cmake .. -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug \
                    -DCMAKE_CXX_FLAGS="$CFLAGS" -DCMAKE_VERBOSE_MAKEFILE=ON \
                    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
                    -DCMAKE_EXE_LINKER_FLAGS="$LFLAGS" $CMAKE_OPT
rc=$?; if [[ $rc != 0 ]]; then exit $rc; fi

scan-build -v -plist --intercept-first --analyze-headers -o $REPORTS_DIR \
           make -j $THREADS -k 2>&1 | tee aff3ct-build.log
rc=$?; if [[ $rc != 0 ]]; then exit $rc; fi

mv aff3ct-build.log $REPORTS_DIR

CLANG_TIDY_MAJOR=$(clang-tidy --version | grep version | awk -F " " '{print $3}' | awk -F "." '{print $1}')
CLANG_TIDY_MINOR=$(clang-tidy --version | grep version | awk -F " " '{print $3}' | awk -F "." '{print $2}')

run-clang-tidy-$CLANG_TIDY_MAJOR.$CLANG_TIDY_MINOR.py -checks='*' -header-filter=.. -p . -j$THREADS > clang-tidy-report.txt
rc=$?; if [[ $rc != 0 ]]; then exit $rc; fi

mv clang-tidy-report.txt $REPORTS_DIR

mv $REPORTS_DIR ../
cd ..
sed -i "s:$PWD:{{ROOT_DIR}}:g" $REPORTS_DIR/aff3ct-build.log
sed -i "s:$PWD:{{ROOT_DIR}}:g" $REPORTS_DIR/clang-tidy-report.txt
sed -i "s:$PWD:{{ROOT_DIR}}:g" $REPORTS_DIR/scan-build-*/*.plist

exit 0;
