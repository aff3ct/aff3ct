@echo on

mkdir build
cd build
cmake .. -G"MinGW Makefiles" -DCMAKE_CXX_COMPILER=g++.exe -DCMAKE_BUILD_TYPE=Release %CMAKE_OPT% -DCMAKE_CXX_FLAGS="-Wall -Wno-misleading-indentation -funroll-loops %CFLAGS%" -DCMAKE_EXE_LINKER_FLAGS="%LFLAGS%"
if %ERRORLEVEL% neq 0 exit /B %ERRORLEVEL%
mingw32-make
if %ERRORLEVEL% neq 0 exit /B %ERRORLEVEL%

mkdir %NAME%
mkdir %NAME%\bin\
mkdir %NAME%\lib\
mkdir %NAME%\inc\

 copy bin\aff3ct.exe  %NAME%\bin\aff3ct.exe
 copy lib\libaff3ct.a %NAME%\lib\aff3ct.lib
xcopy ..\src\*        %NAME%\inc\ /s /e

move %NAME% ..\
