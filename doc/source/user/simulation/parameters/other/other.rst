.. _other-other-parameters:

Other parameters
----------------

.. _other-help:

``--help, -h``
""""""""""""""

|factory::Launcher::help,h|

.. code-block:: bash

   aff3ct -h

.. code-block:: console

   Usage: ./bin/aff3ct -C <text> [optional args...]

   Simulation parameter(s):
   {R} --sim-cde-type, -C <text:including set={BCH|LDPC|POLAR|RA|REP|RS|RSC|RSC_DB|TPC|TURBO|TURBO_DB|UNCODED}>
         Select the channel code family to simulate.
       --sim-prec, -p     <integer:including set={8|16|32|64}>
         Specify the representation of the real numbers in the receiver part of the
         chain.
       --sim-type         <text:including set={BFER|BFERI}>
         Select the type of simulation (or communication chain skeleton).

   Other parameter(s):
       --Help, -H
         Print the help like with the '--help, -h' parameter plus advanced
         arguments (denoted as '{A}').
       --help, -h
         Print the help with all the required (denoted as '{R}') and optional
         arguments. The latter change depending on the selected simulation type and
         code.
       --no-colors
         Disable the colors in the shell.
       --version, -v
         Print informations about the version of the source code and compilation
         options.

.. _other-advanced-help:

``--Help, -H``
""""""""""""""

|factory::Launcher::Help,H|

.. code-block:: bash

   aff3ct -H

.. code-block:: console
   :emphasize-lines: 16,17,18,19,20,21,22,23,28,29,32,33,34

   Usage: ./bin/aff3ct -C <text> [optional args...]

   Simulation parameter(s):
   {R} --sim-cde-type, -C <text:including set={BCH|LDPC|POLAR|RA|REP|RS|RSC|RSC_DB|TPC|TURBO|TURBO_DB|UNCODED}>
         Select the channel code family to simulate.
       --sim-prec, -p     <integer:including set={8|16|32|64}>
         Specify the representation of the real numbers in the receiver part of the
         chain.
       --sim-type         <text:including set={BFER|BFERI}>
         Select the type of simulation (or communication chain skeleton).

   Other parameter(s):
       --Help, -H
         Print the help like with the '--help, -h' parameter plus advanced
         arguments (denoted as '{A}').
   {A} --except-no-bt
         Disable the backtrace display when running an exception.
   {A} --full-legend
         Display the legend with all modules details when launching the simulation.
       --help, -h
         Print the help with all the required (denoted as '{R}') and optional
         arguments. The latter change depending on the selected simulation type and
         code.
   {A} --keys, -k
         Display the parameter keys in the help.
       --no-colors
         Disable the colors in the shell.
   {A} --no-legend
         Disable the legend display (remove all the lines beginning by the '#'
         character).
       --version, -v
         Print informations about the version of the source code and compilation
         options.

.. _other-version:

``--version, -v``
"""""""""""""""""

|factory::Launcher::version,v|

.. code-block:: bash

   aff3ct -v

.. code-block:: console

   aff3ct (Linux 64-bit, g++-5.4, AVX2) v2.1.1-48-g1c72c3d
   Compilation options:
     * Precision: 8/16/32/64-bit
     * Polar bit packing: on
     * Terminal colors: on
     * Backtrace: on
     * External strings: on
     * MPI: off
     * GSL: off
     * MKL: off
   Copyright (c) 2016-2018 - MIT license.
   This is free software; see the source for copying conditions.  There is NO
   warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

.. _other-keys:

``--keys, -k`` |image_advanced_argument|
""""""""""""""""""""""""""""""""""""""""

|factory::Launcher::keys,k|

.. code-block:: bash

   aff3ct -h -k

.. code-block:: console
   :emphasize-lines: 5,8,12,17,21,26,29

   Usage: ./bin/aff3ct -C <text> [optional args...]

   Simulation parameter(s):
   {R} --sim-cde-type, -C <text:including set={BCH|LDPC|POLAR|RA|REP|RS|RSC|RSC_DB|TPC|TURBO|TURBO_DB|UNCODED}>
         [factory::Launcher::p+cde-type,C]
         Select the channel code family to simulate.
       --sim-prec, -p     <integer:including set={8|16|32|64}>
         [factory::Launcher::p+prec,p]
         Specify the representation of the real numbers in the receiver part of the
         chain.
       --sim-type         <text:including set={BFER|BFERI}>
         [factory::Launcher::p+type]
         Select the type of simulation (or communication chain skeleton).

   Other parameter(s):
       --Help, -H
         [factory::Launcher::Help,H]
         Print the help like with the '--help, -h' parameter plus advanced
         arguments (denoted as '{A}').
       --help, -h
         [factory::Launcher::help,h]
         Print the help with all the required (denoted as '{R}') and optional
         arguments. The latter change depending on the selected simulation type and
         code.
       --no-colors
         [factory::Launcher::no-colors]
         Disable the colors in the shell.
       --version, -v
         [factory::Launcher::version,v]
         Print informations about the version of the source code and compilation
         options.

.. _other-except-no-bt:

``--except-no-bt`` |image_advanced_argument|
""""""""""""""""""""""""""""""""""""""""""""

|factory::Launcher::except-no-bt|

.. _sim-no-legend:

``--no-legend`` |image_advanced_argument|
"""""""""""""""""""""""""""""""""""""""""

|factory::Launcher::no-legend|

.. tip:: Use this option when you want to complete an already existing
   simulation result file with new noise points. Pay attention to use ``>>``
   instead of ``>`` to redirect the standard output in order to add results at
   the end of the file and not overwriting it.

.. _sim-full-legend:

``--full-legend`` |image_advanced_argument|
"""""""""""""""""""""""""""""""""""""""""""

|factory::Launcher::full-legend|

This additional information can help to understand a problem in the simulation.
Data can of course be redundant from one module to another.

.. _sim-no-colors:

``--no-colors``
"""""""""""""""

|factory::Launcher::no-colors|
