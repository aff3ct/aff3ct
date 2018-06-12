@echo on

set PATH=%PATH%;C:\Program Files\Git\cmd
set PATH=%PATH%;C:\Program Files\CMake\bin
set PATH=%PATH%;C:\Program Files\Git\mingw64\bin
mkdir build
cd build
cmake .. -G"MinGW Makefiles" -DCMAKE_CXX_COMPILER=g++.exe -DCMAKE_BUILD_TYPE=Release -DENABLE_EXEC=ON -DENABLE_STATIC_LIB=ON -DCMAKE_CXX_FLAGS="-Wall -Wno-misleading-indentation -funroll-loops -msse4.2 -DMULTI_PREC" -DCMAKE_EXE_LINKER_FLAGS="-static -static-libgcc -static-libstdc++"
mingw32-make

mkdir build_windows_gcc_sse4.2
mkdir build_windows_gcc_sse4.2\bin\
mkdir build_windows_gcc_sse4.2\lib\

copy bin\aff3ct.exe  build_windows_gcc_sse4.2\bin\aff3ct.exe
copy lib\libaff3ct.a build_windows_gcc_sse4.2\lib\aff3ct.lib

move build_windows_gcc_sse4.2 ..\
