.. _global-other-parameters:

Other parameters
----------------

.. _global-help:

``--help, -h``
""""""""""""""

Print the help with all the required ``{R}`` and optional arguments.
The latter change in function of the selected simulation type and code.

.. code-block:: console

   $ aff3ct -h
   Usage: ./bin/aff3ct -C <text> [optional args...]

   Simulation parameter(s):
   {R} --sim-cde-type, -C <text:including set={BCH|LDPC|POLAR|RA|REP|RS|RSC|RSC_DB|TURBO|TURBO_DB|TURBO_PROD|UNCODED}>
         select the code type you want to use.
       --sim-no-colors
         disable the colors in the shell.
       --sim-prec, -p     <integer:including set={8|16|32|64}>
         the simulation precision in bits.
       --sim-type         <text:including set={BFER|BFERI|EXIT}>
         select the type of simulation to launch (default is BFER).

   Other parameter(s):
       --Help, -H
         print this help with the advanced arguments.
       --help, -h
         print this help.
       --version, -v
         print informations about the version of the code.

.. _global-advanced-help:

``--Help, -H``
""""""""""""""

Print the help with in more the advanced ``{A}`` arguments.

.. code-block:: console

   $ aff3ct -H
   Usage: ./bin/aff3ct -C <text> [optional args...]

   Simulation parameter(s):
   {R} --sim-cde-type, -C <text:including set={BCH|LDPC|POLAR|RA|REP|RS|RSC|RSC_DB|TURBO|TURBO_DB|TURBO_PROD|UNCODED}>
         select the code type you want to use.
       --sim-no-colors
         disable the colors in the shell.
   {A} --sim-no-legend
         Do not display any legend when launching the simulation.
       --sim-prec, -p     <integer:including set={8|16|32|64}>
         the simulation precision in bits.
       --sim-type         <text:including set={BFER|BFERI|EXIT}>
         select the type of simulation to launch (default is BFER).

   Other parameter(s):
       --Help, -H
         print this help with the advanced arguments.
   {A} --except-no-bt
         do not print the backtrace when displaying exception.
       --help, -h
         print this help.
       --version, -v
         print informations about the version of the code.

.. _global-version:

``--version, -v``
"""""""""""""""""

Print informations about the version of the source code and compilation options.

.. code-block:: console

   $ aff3ct -v
   aff3ct (Linux 64-bit, g++ 8.1, AVX2) v2.0.0-112-gc74ce62
   Copyright (c) 2016-2018 - MIT license.
   This is free software; see the source for copying conditions.  There is NO
   warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

.. _global-except-a2l:

``--except-a2l`` |image_advanced_argument|
""""""""""""""""""""""""""""""""""""""""""

Enhance the backtrace when displaying exception. This change the program
addresses into filenames and lines. It may take some seconds to do this work.

.. note:: This option works only on Unix based |OS| and if |AFF3CT| has been
   :ref:`compiled <compilation_compiler_options>` with debug symbols
   (``-g`` compile flag) and **without** ``NDEBUG`` macro (``-DNDEBUG`` flag).

.. _global-except-no-bt:

``--except-no-bt`` |image_advanced_argument|
""""""""""""""""""""""""""""""""""""""""""""

Disable the print of backtrace when displaying exception.
