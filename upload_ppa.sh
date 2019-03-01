#!/bin/bash
rm -rf ~/Projects/aff3ct/build/Debian/
cd  ~/Projects/aff3ct/build/
cmake .. -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-funroll-loops -msse4.1"
make -j8
cd Debian/xenial/
cpack -G TGZ --config ../../CPackSourceConfig.cmake
cd aff3ct_*~xenial1
tar zxf ../aff3ct_2.3.*.orig.tar.gz 
debuild --no-tgz-check -S 
cd ..
dput ppa:mathieu-leonardon/aff3ct aff3ct_2.3.*~xenial1_source.changes
