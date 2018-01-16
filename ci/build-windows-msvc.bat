@echo on

set PATH=%PATH%;C:\Program Files\Git\cmd
set PATH=%PATH%;C:\Program Files\CMake\bin
set PATH=%PATH%;C:\Program Files\Git\mingw64\bin
set "VSCMD_START_DIR=%CD%"
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars64.bat"
mkdir build
cd build
cmake .. -G"Visual Studio 15 2017 Win64" -DCMAKE_CXX_FLAGS="-D_CRT_SECURE_NO_DEPRECATE -DMULTI_PREC /EHsc /arch:AVX"
devenv /build Release aff3ct.sln

cd bin\Release
aff3ct.exe --version
aff3ct.exe -C POLAR -m 1.0 -M 3.5 -s 0.25 -K 1755 -N 2048 --crc-poly 32-GZIP --dec-type ASCL --ter-freq 0