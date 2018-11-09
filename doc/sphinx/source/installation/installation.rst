Installation
============

.. important:: If you do not plan to modify the AFF3CT source code and you want
               to use the simulator as is, you can **download one of the lastest
               AFF3CT builds** from the
               `download page of the AFF3CT website <http://aff3ct.github.io/download.html>`_
               and skip this section.

Once AFF3CT has been compiled, it is possible (not mandatory) to install it on
your system. On Unix-like systems, traditionally, the fresh build is installed
in the ``/usr/local`` directory. This location can be changed by setting the
``CMAKE_INSTALL_PREFIX`` built-in variable with an other path. For instance, to
install AFF3CT in the current build:

.. code-block:: bash

   cmake .. -DCMAKE_INSTALL_PREFIX="install"

This command do not install AFF3CT. It only prepares the project to be installed
in the selected location.

Makefile Project
----------------

To install AFF3CT, call the `install` target on the current Makefile:

.. code-block:: bash

   make install

.. note:: Depending on the chosen ``CMAKE_INSTALL_PREFIX`` location, the
          administrator privileges (**sudo**) can be required.


Visual Studio Solution
----------------------

In case of a Visual Studio Solution, an `INSTALL` project is defined and ensures
the installation when triggered. This can be done from the Visual Studio IDE
or from the command line after calling the
``%VS_PATH%\VC\Auxiliary\Build\vcvars64.bat`` batch script (where ``%VS_PATH%``
is the location of Visual Studio on your system):

.. code-block:: bash

   devenv /build Release aff3ct.sln /project INSTALL

Details
-------

The installed package is organized as follow:

* ``bin/``

  - ``aff3ct-M.m.p/`` contains the AFF3CT executable binary.

* ``include/``

  - ``aff3ct-M.m.p/`` contains all the includes required by AFF3CT.

* ``lib/``

  - ``aff3ct-M.m.p/`` contains the AFF3CT library.
  - ``cmake/``

     * ``aff3ct-M.m.p/`` contains the CMake configuration files required to link with AFF3CT.

* ``share/``

  - ``aff3ct-M.m.p``

     * ``conf/`` contains some input files to configure the AFF3CT simulator.
     * ``refs/`` many results from AFF3CT simulations.

``M`` stands for the major number of the version, ``m`` the minor number and
``p`` the id of the last patch.