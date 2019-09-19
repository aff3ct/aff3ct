.. _user_installation_guide_installation:

Installation
============

In order to be installed on a system, AFF3CT can either be compiled locally
and installed (see `From Source`_), or remotely precompiled versions
can be downloaded and installed (see `Precompiled Versions`_.)

From Source
-----------

Once |AFF3CT| has been compiled, it is possible (not mandatory) to install it on
your system. On Unix-like systems, traditionally, the fresh build is installed
in the ``/usr/local`` directory (this is the CMake default installation path).
This location can be changed by setting the ``CMAKE_INSTALL_PREFIX`` built-in
variable with an other path. For instance, to install |AFF3CT| in the current
build:

.. code-block:: bash

   cmake .. -DCMAKE_INSTALL_PREFIX="install"

This command do not install |AFF3CT|. It only prepares the project to be
installed in the selected location.

Makefile Project
""""""""""""""""

To install |AFF3CT|, call the `install` target on the current Makefile:

.. code-block:: bash

   make install

.. note:: Depending on the chosen ``CMAKE_INSTALL_PREFIX`` location, the
          administrator privileges (**sudo**) can be required.


Visual Studio Solution
""""""""""""""""""""""

In case of a Visual Studio Solution, an `INSTALL` project is defined and ensures
the installation when triggered. This can be done from the Visual Studio IDE
or from the command line after calling the
``%VS_PATH%\VC\Auxiliary\Build\vcvars64.bat`` batch script (where ``%VS_PATH%``
is the location of Visual Studio on your system):

.. code-block:: bash

   devenv /build Release aff3ct.sln /project INSTALL

Precompiled Versions
--------------------

From |AFF3CT| website
"""""""""""""""""""""
If you do not plan to modify the |AFF3CT| source code and you
want to use the simulator/library as is, you can **download one
of the latest builds** from the
`download page of the AFF3CT website <http://aff3ct.github.io/download.html>`_.
Precompiled binaries are available for the most common operating systems :
Windows, macOS and Linux.

On Debian / Ubuntu
""""""""""""""""""
Each new version of |AFF3CT| is deployed on |PPA| repositories for the aptitude
package manager. Two different repositories are available. The first one,
``stable``, holds versions that are released after a lot of testing to ensure
performance and stability. The second one, ``dev``, holds the latest development
versions of |AFF3CT|.

Select the channel to use (``stable`` **or** ``dev``, **not both!**):

.. code-block:: bash

   # stable
   sudo add-apt-repository ppa:aff3ct/aff3ct-stable

   # dev
   sudo add-apt-repository ppa:aff3ct/aff3ct-dev

Update package list and install:

.. code-block:: bash

   sudo apt-get update
   sudo apt-get install aff3ct-bin aff3ct-doc libaff3ct libaff3ct-dev

* The package ``aff3ct-bin`` contains the ``bin/``, ``conf/`` and ``refs/`` folders.

* The package ``aff3ct-doc`` contains the ``doc/`` folder.

* The package ``libaff3ct`` contains the ``lib/`` folder.

* The package ``libaff3ct-dev`` contains the ``include/`` folder and depends on the ``libaff3ct`` package.


Contents
--------

The installed package is organized as follow:

* ``bin/``

  - ``aff3ct-M.m.p`` the |AFF3CT| executable binary.

* ``include/``

  - ``aff3ct-M.m.p/`` contains all the includes required by |AFF3CT|.

* ``lib/``

  - ``libaff3ct-M.m.p.a`` the |AFF3CT| static library.
  - ``libaff3ct-M.m.p.so`` the |AFF3CT| shared library.
  - ``cmake/``

     * ``aff3ct-M.m.p/`` contains the CMake configuration files required to link with |AFF3CT|.

* ``share/``

  - ``aff3ct-M.m.p``

     * ``conf/`` contains some input files to configure the |AFF3CT| simulator.
     * ``refs/`` many results from |AFF3CT| simulations.
     * ``doc/`` contains the |AFF3CT| documentation.

``M`` stands for the major number of the version, ``m`` the minor number and
``p`` the id of the last patch.