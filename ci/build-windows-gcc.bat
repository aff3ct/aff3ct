@echo on

set PATH=%PATH%;C:\Program Files\Git\cmd
set PATH=%PATH%;C:\Program Files\CMake\bin
set PATH=%PATH%;C:\Program Files\Git\mingw64\bin
mkdir build
cd build
cmake .. -G"MinGW Makefiles" -DCMAKE_CXX_COMPILER=g++.exe -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-Wall -Wno-misleading-indentation -funroll-loops -march=native -DMULTI_PREC"
mingw32-make

cd bin
aff3ct.exe --version
aff3ct.exe -C POLAR -m 1.0 -M 3.5 -s 0.25 -K 1755 -N 2048 --crc-poly 32-GZIP --dec-type ASCL --ter-freq 0