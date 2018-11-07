@echo on

set "VSCMD_START_DIR=%CD%"
call "%VS_PATH%\VC\Auxiliary\Build\vcvars64.bat"

mkdir build
cd build
cmake .. -G"Visual Studio 15 2017 Win64" %CMAKE_OPT% -DCMAKE_CXX_FLAGS="%CFLAGS% /MP%THREADS%"
if %ERRORLEVEL% neq 0 exit /B %ERRORLEVEL%
devenv /build Release aff3ct.sln
rem msbuild aff3ct.sln /t:Build /p:Configuration=Release
if %ERRORLEVEL% neq 0 exit /B %ERRORLEVEL%

mkdir %NAME%
mkdir %NAME%\bin\
mkdir %NAME%\lib\
mkdir %NAME%\include\
mkdir %NAME%\include\aff3ct\

 copy bin\Release\aff3ct.exe %NAME%\bin\
 copy lib\Release\aff3ct.lib %NAME%\lib\
xcopy ..\src\*               %NAME%\include\aff3ct\ /s /e

move %NAME% ..\
