# Install on Windows

## Download & install Git

> https://git-scm.com/download/win

Next next next... Install!

## Download & install MinGW64

> https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win64/Personal%20Builds/mingw-builds/6.2.0/threads-posix/seh/x86_64-6.2.0-release-posix-seh-rt_v5-rev1.7z

Unzip the archive.
Copy the mingw64 folder in the /C/Programmes/Git/mingw64 folder (erase all duplicated files).

## Download & install CMake

> https://cmake.org/files/v3.7/cmake-3.7.1-win64-x64.msi

Next next next... Install!

Launch `Git Bash`.
Add cmake to the PATH:

	$ echo '#!/bin/bash' >> ~/.bashrc
	$ echo 'export PATH=$PATH:"C:\Program Files\CMake\bin"' >> ~/.bashrc
	$ source ~/.bashrc

## Install AFF3CT

	$ cd ~/
	$ git clone https://github.com/aff3ct/aff3ct.git
	$ cd aff3ct
	$ mkdir build
	$ cd build
	$ cmake .. -G"MinGW Makefiles"
	$ cmake .. -DCMAKE_CXX_COMPILER=g++.exe -DCMAKE_CC_COMPILER=gcc.exe -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-Wall -funroll-loops -DMULTI_PREC -march=native" -DCMAKE_CXX_FLAGS_RELEASE="-Ofast"
	$ mingw32-make -j2

## Install Python 3

> https://www.python.org/ftp/python/3.6.0/python-3.6.0.exe
	
Next next next... Install!
	
	$ echo 'export PATH=$PATH:~/AppData/Local/Programs/Python/Python36-32/' >> ~/.bashrc
	$ echo 'export PATH=$PATH:~/AppData/Local/Programs/Python/Python36-32/Scripts/' >> ~/.bashrc
	$ source ~/.bashrc
	$ pip3.6 install pyqt5
	$ pip3.6 install numpy

## Run PyBER

	$ cd ~/aff3ct/plotter/PyBER/
	$ python pyBER.py &
