@echo on

set PATH=%PATH%;C:\Program Files\Git\cmd
set PATH=%PATH%;C:\Program Files\CMake\bin
set PATH=%PATH%;C:\Program Files\Git\mingw64\bin
set "VSCMD_START_DIR=%CD%"
call "vcvars64.bat"
mkdir build
cd build
cmake .. -G"Visual Studio 15 2017 Win64" %CMAKE_OPT% -DCMAKE_CXX_FLAGS="-D_CRT_SECURE_NO_DEPRECATE /MT /EHsc %CFLAGS%"
if %ERRORLEVEL% neq 0 exit /B %ERRORLEVEL%
devenv /build Release aff3ct.sln
if %ERRORLEVEL% neq 0 exit /B %ERRORLEVEL%

mkdir %NAME%
mkdir %NAME%\bin\
mkdir %NAME%\lib\
mkdir %NAME%\inc\

 copy bin\Release\aff3ct.exe %NAME%\bin\
 copy lib\Release\aff3ct.lib %NAME%\lib\
xcopy ..\src\*               %NAME%\inc\ /s /e

move %NAME% ..\
