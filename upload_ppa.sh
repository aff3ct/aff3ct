#!/bin/bash
rm -rf ~/Projects/aff3ct/build/Debian/
cd  ~/Projects/aff3ct/build/
cmake .. -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-funroll-loops -march=native"
cd Debian/xenial/
cpack -G TGZ --config ../../CPackSourceConfig.cmake
cd aff3ct_*~xenial1
tar zxf ../aff3ct_2.3.*.orig.tar.gz 
debuild --no-tgz-check -ICVS -nc  -S 
cd ..
dput ppa:mathieu-leonardon/aff3ct aff3ct_2.3.*~xenial1_source.changes
