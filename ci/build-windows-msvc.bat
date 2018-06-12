@echo on

set PATH=%PATH%;C:\Program Files\Git\cmd
set PATH=%PATH%;C:\Program Files\CMake\bin
set PATH=%PATH%;C:\Program Files\Git\mingw64\bin
set "VSCMD_START_DIR=%CD%"
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars64.bat"
mkdir build
cd build
cmake .. -G"Visual Studio 15 2017 Win64" -DENABLE_EXEC=ON -DENABLE_STATIC_LIB=ON -DCMAKE_CXX_FLAGS="-D_CRT_SECURE_NO_DEPRECATE -DMULTI_PREC /MT /EHsc /arch:AVX"
devenv /build Release aff3ct.sln

mkdir build_windows_msvc_avx
mkdir build_windows_msvc_avx\bin\
mkdir build_windows_msvc_avx\lib\

copy bin\Release\aff3ct.exe    build_windows_msvc_avx\bin\
copy lib\Release\libaff3ct.lib build_windows_msvc_avx\lib\

move build_windows_msvc_avx ..\
