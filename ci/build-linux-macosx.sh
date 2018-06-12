#!/bin/bash
set -x

mkdir build
cd build

cmake .. -G"Unix Makefiles" -DCMAKE_CXX_COMPILER=$CC -DCMAKE_CXX_COMPILER=$CXX \
         -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-Wall -funroll-loops -DENABLE_COOL_BASH $CFLAGS" \
         -DENABLE_EXEC=ON -DENABLE_STATIC_LIB=ON -DENABLE_SHARED_LIB=ON $OPT
rc=$?; if [[ $rc != 0 ]]; then exit $rc; fi

make -j $THREADS
rc=$?; if [[ $rc != 0 ]]; then exit $rc; fi

mkdir $NAME $NAME/bin $NAME/lib
cp bin/aff3ct $NAME/bin/aff3ct
cp lib/libaff3ct.a $NAME/lib/libaff3ct.a
tar -zcvf $NAME.tar.gz $NAME/
mv $NAME.tar.gz ../
