Linux/MacOS
-----------------

Install the environment
^^^^^^^^^^^^^^^^^^^^^^^

.. note::
   To install programs, on Debian-like systems use ``apt``.
   On CentOS-like use ``yum``.
   On MacOS use ``brew``.

.. warning::
   You may need to have super user rights, then use **sudo**.


AFF3CT is a ``cmake`` project, so you need to install it with ``make``:

.. code-block:: bash

   apt install cmake make

``Git`` is used to manage code versions so to download it from the repository:

.. code-block:: bash

   apt install git


You will then need a compiler to generate the executable. AFF3CT is tested with
``clang++``, ``g++`` and ``icpc``:

.. code-block:: bash

   apt install g++



Clone and compile AFF3CT
^^^^^^^^^^^^^^^^^^^^^^^^

Get the source code (AFF3CT's + submodules') from the ``git`` repository:

.. code-block:: bash

   git clone https://github.com/aff3ct/aff3ct.git
   cd aff3ct
   git submodule update --init --recursive


Generate the *Makefile* with ``cmake``:

.. code-block:: bash

   mkdir build
   cd build
   cmake .. -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-funroll-loops -march=native -DMULTI_PREC -DENABLE_COOL_BASH"

And finally, compile with `make <https://www.gnu.org/software/make/manual/html_node/Options-Summary.html>`_:

.. code-block:: bash

   make -j4
