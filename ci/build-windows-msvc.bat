@echo on

set "VSCMD_START_DIR=%CD%"
call "%VS_PATH%\VC\Auxiliary\Build\vcvars64.bat"

cmake --version
mkdir build
cd build
cmake .. -G"Visual Studio 15 2017 Win64" %CMAKE_OPT% -DCMAKE_CXX_FLAGS="%CFLAGS% /MP%THREADS%" -DCMAKE_INSTALL_PREFIX="%NAME%"
if %ERRORLEVEL% neq 0 exit /B %ERRORLEVEL%
devenv /build Release aff3ct.sln
rem msbuild aff3ct.sln /t:Build /p:Configuration=Release
if %ERRORLEVEL% neq 0 exit /B %ERRORLEVEL%
rd /s /q %NAME%
devenv /build Release aff3ct.sln /project INSTALL > nul
if %ERRORLEVEL% neq 0 exit %ERRORLEVEL%
move %NAME% ..\
