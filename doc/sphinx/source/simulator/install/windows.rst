On Windows
-------------------------

Download & install Git
^^^^^^^^^^^^^^^^^^^^^^

| Download and install `Git <https://git-scm.com/downloads>`_
| Next next next... Install!


Download & install MinGW64
^^^^^^^^^^^^^^^^^^^^^^^^^^

| Download `MinGW <https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win64/Personal%20Builds/mingw-builds/6.2.0/threads-posix/seh/x86_64-6.2.0-release-posix-seh-rt_v5-rev1.7z>`_
| Tested with: `MinGW x86_64-6.2.0 <https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win64/Personal%20Builds/mingw-builds/6.2.0/threads-posix/seh/x86_64-6.2.0-release-posix-seh-rt_v5-rev1.7z>`_

Unzip the archive. Copy the extracted ``mingw64`` folder in the
``C:\Programs\Git\folder`` (erase all the duplicated files).


Download & install CMake
^^^^^^^^^^^^^^^^^^^^^^^^

| Download and install this file: https://cmake.org/files/v3.7/cmake-3.7.1-win64-x64.msi
| Next next next... Install!

Launch ``Git Bash`` (from the ``Start Menu -> Git -> Git Bash`` link).
Add cmake to the PATH:

.. code-block:: bash

   echo '#!/bin/bash' >> ~/.bashrc
   echo 'export PATH=$PATH:"C:\Program Files\CMake\bin"' >> ~/.bashrc
   source ~/.bashrc


Compile AFF3CT
^^^^^^^^^^^^^^

.. code-block:: bash

   cd ~/
   git clone https://github.com/aff3ct/aff3ct.git
   cd aff3ct
   git submodule update --init --recursive
   mkdir build
   cd build
   cmake .. -G"MinGW Makefiles"

This last command line (``cmake .. -G"MinGW Makefiles"``) should fail
but this is not important, continue with:

.. code-block:: bash

   cmake .. -DCMAKE_CXX_COMPILER=g++.exe -DCMAKE_CC_COMPILER=gcc.exe -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-Wall -funroll-loops -march=native -DMULTI_PREC"
   mingw32-make -j4
