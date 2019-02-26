@echo on

cmake --version
mkdir build
cd build
cmake .. -G"MinGW Makefiles" -DCMAKE_CXX_COMPILER=g++.exe -DCMAKE_BUILD_TYPE=Release %CMAKE_OPT% -DCMAKE_CXX_FLAGS="%CFLAGS%" -DCMAKE_EXE_LINKER_FLAGS="%LFLAGS%" -DCMAKE_INSTALL_PREFIX="%NAME%"
if %ERRORLEVEL% neq 0 exit /B %ERRORLEVEL%
mingw32-make -j %THREADS%
if %ERRORLEVEL% neq 0 exit /B %ERRORLEVEL%
rd /s /q %NAME%
mingw32-make install > nul
if %ERRORLEVEL% neq 0 exit /B %ERRORLEVEL%
move %NAME% ..\
