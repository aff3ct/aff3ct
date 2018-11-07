Installation
============

Once AFF3CT has been compiled, it is possible (not mandatory) to install it on
your system. On Unix-like systems, traditionally, the fresh build is installed
in the ``/usr/local`` directory. This location can be changed by setting the
``CMAKE_INSTALL_PREFIX`` built-in variable with an other path. For instance if
you want to install AFF3CT in your current build you can do:

.. code-block:: bash

   cmake .. -DCMAKE_INSTALL_PREFIX="install"

This command do not install AFF3CT. It only prepares the project to be installed
in the selected location.

Makefile Project
----------------

To install AFF3CT you will need to call the `install` target on the current
Makefile:

.. code-block:: bash

   make install

.. note:: Depending on the ``CMAKE_INSTALL_PREFIX`` location you chose you may
          need to have the administrator privileges (**sudo**).


Visual Studio Solution
----------------------

In case of Visual Studio Solution, an `INSTALL` project is defined and ensures
the installation when triggered. This can be done from the Visual Studio IDE
or from the command line after calling the
``%VS_PATH%\VC\Auxiliary\Build\vcvars64.bat`` batch script (where ``%VS_PATH%``
is the location of Visual Studio on your system):

.. code-block:: bash

   devenv /build Release aff3ct.sln /project INSTALL

Details
-------

The installed packaged is organized as follow:

* ``bin/``

  - ``aff3ct-M.m.p/``

* ``include/``

  - ``aff3ct-M.m.p/``

* ``lib/``

  - ``aff3ct-M.m.p/``
  - ``cmake/``

     * ``aff3ct-M.m.p/``

* ``share/``

  - ``aff3ct-M.m.p``

     * ``conf/``
     * ``refs/``

``M`` stands for the major number of the version, ``m`` the minor number and
``p`` the id of the last patch.
The ``bin/aff3ct-M.m.p/`` directory contains the AFF3CT executable binary.
The ``include/aff3ct-M.m.p/`` directory contains all the includes required by
AFF3CT.
The ``lib/aff3ct-M.m.p/`` directory contains the AFF3CT library.
The ``lib/cmake/aff3ct-M.m.p/`` directory contains the CMake configuration files
required to link with AFF3CT.
The ``share/aff3ct-M.m.p/conf/`` directory contains some input files to
configure the AFF3CT simulator.
The ``share/aff3ct-M.m.p/refs/`` directory contains many results from AFF3CT
simulations.