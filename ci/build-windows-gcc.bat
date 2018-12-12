@echo on

mkdir build
cd build
cmake .. -G"MinGW Makefiles" -DCMAKE_CXX_COMPILER=g++.exe -DCMAKE_BUILD_TYPE=Release %CMAKE_OPT% -DCMAKE_CXX_FLAGS="%CFLAGS%" -DCMAKE_EXE_LINKER_FLAGS="%LFLAGS%"
if %ERRORLEVEL% neq 0 exit /B %ERRORLEVEL%
mingw32-make -j %THREADS%
if %ERRORLEVEL% neq 0 exit /B %ERRORLEVEL%

rd /s /q %NAME%
mkdir %NAME%
mkdir %NAME%\bin\
mkdir %NAME%\lib\
mkdir %NAME%\include\
mkdir %NAME%\include\aff3ct\

 copy bin\aff3ct*.exe  %NAME%\bin\
 copy lib\libaff3ct*.a %NAME%\lib\
xcopy ..\src\*         %NAME%\include\aff3ct\ /s /e > nul

move %NAME% ..\
