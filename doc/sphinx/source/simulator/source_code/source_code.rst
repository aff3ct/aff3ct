Source Code
===========

.. _Git: https://en.wikipedia.org/wiki/Git
.. _GitHub: https://github.com/
.. _AFF3CT repository: https://github.com/aff3ct/aff3ct

This project uses `Git`_ as the version-control system to manage the source
code. The AFF3CT repository is hosted on `GitHub`_. To get the source code,
first you need to install the Git software and secondly you have to `clone` the
`AFF3CT repository`_ locally.

Git Installation
----------------

.. _source_code-git_installation-windows:

Windows/macOS
^^^^^^^^^^^^^

Download Git from `the official web page <https://git-scm.com/downloads>`_
and launch the executable file. Just press the `Next` button until the
installation is over.


.. note:: On Windows, during the installation you may want to check the **Linux
          symbolic links** support.

.. warning:: We recommend to add Git in your system PATH during the
             installation.

.. warning:: On Windows, Git comes with the **Git Bash** terminal which is,
             to our mind, better suitable that the traditional **Windows
             Console**. We encourage you to use **Git Bash** instead of the
             **Windows Console** for the following steps.

Linux
^^^^^

Install Git from your package manager:

.. code-block:: bash

   sudo apt install git

.. note:: On CentOS-like systems you have to replace ``apt`` by ``yum``.

Clone AFF3CT from GitHub
------------------------

From your favorite terminal:

.. code-block:: bash

   git clone https://github.com/aff3ct/aff3ct.git
   cd aff3ct

.. _Git submodule feature: https://git-scm.com/docs/git-submodule

The AFF3CT repository contains some dependencies to other repositories.
Technically those dependencies are managed by the `Git submodule feature`_.
By default the submodules are not downloaded during the ``git clone`` process.
Before doing anything else it is highly recommended to get those submodules:

.. code-block:: bash

   git submodule update --init --recursive

.. danger:: On the `AFF3CT repository`_ you may want to directly download
            the source code without making a ``git clone``. This will get you an
            archive without the AFF3CT dependencies and the build process will
            fail. **Do not directly download AFF3CT from GitHub and please make
            a clone!**