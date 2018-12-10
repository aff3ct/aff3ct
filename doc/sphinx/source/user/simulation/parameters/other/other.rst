.. _other-other-parameters:

Other parameters
----------------

.. _other-help:

``--help, -h``
""""""""""""""

**STRING**: |factory::Launcher::parameters::help,h|

Print the help with all the required ``{R}`` and optional arguments.
The latter change in function of the selected simulation type and code.

.. code-block:: bash

   aff3ct -h

.. code-block:: console

   Usage: ./bin/aff3ct -C <text> [optional args...]

   Simulation parameter(s):
   {R} --sim-cde-type, -C <text:including set={BCH|LDPC|POLAR|RA|REP|RS|RSC|RSC_DB|TPC|TURBO|TURBO_DB|UNCODED}>
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

.. _other-advanced-help:

``--Help, -H``
""""""""""""""

**STRING**: |factory::Launcher::parameters::Help,H|

Print the help with in more the advanced ``{A}`` arguments.

.. code-block:: bash

   aff3ct -H

.. code-block:: console
   :emphasize-lines: 8,9,18,19,22,23

   Usage: ./bin/aff3ct -C <text> [optional args...]

   Simulation parameter(s):
   {R} --sim-cde-type, -C <text:including set={BCH|LDPC|POLAR|RA|REP|RS|RSC|RSC_DB|TPC|TURBO|TURBO_DB|UNCODED}>
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
   {A} --keys, -k
         display the parameter keys in the help.
       --version, -v
         print informations about the version of the code.

.. _other-keys:

``--keys, -k``
"""""""""""""""""

**STRING**: |factory::Launcher::parameters::keys,k|

.. code-block:: bash

   aff3ct -H -k

.. code-block:: console
   :emphasize-lines: 5,8,11,14,17,22,25,28,31,34

   Usage: ./bin/aff3ct -C <text> [optional args...]

   Simulation parameter(s):
   {R} --sim-cde-type, -C <text:including set={BCH|LDPC|POLAR|RA|REP|RS|RSC|RSC_DB|TPC|TURBO|TURBO_DB|UNCODED}>
         [factory::Launcher::parameters::p+cde-type,C]
         select the code type you want to use.
       --sim-no-colors
         [factory::Launcher::parameters::p+no-colors]
         disable the colors in the shell.
   {A} --sim-no-legend
         [factory::Launcher::parameters::p+no-legend]
         do not display any legend when launching the simulation.
       --sim-prec, -p     <integer:including set={8|16|32|64}>
         [factory::Launcher::parameters::p+prec,p]
         the simulation precision in bits.
       --sim-type         <text:including set={BFER|BFERI|EXIT}>
         [factory::Launcher::parameters::p+type]
         select the type of simulation to launch (default is BFER).

   Other parameter(s):
       --Help, -H
         [factory::Launcher::parameters::Help,H]
         print this help with the advanced arguments.
   {A} --except-no-bt
         [factory::Launcher::parameters::except-no-bt]
         do not print the backtrace when displaying exception.
       --help, -h
         [factory::Launcher::parameters::help,h]
         print this help.
   {A} --keys, -k
         [factory::Launcher::parameters::keys,k]
         display the parameter keys in the help.
       --version, -v
         [factory::Launcher::parameters::version,v]
         print informations about the version of the code.

.. _other-version:

``--version, -v``
"""""""""""""""""

**STRING**: |factory::Launcher::parameters::version,v|

Print informations about the version of the source code and compilation options.

.. code-block:: bash

   aff3ct -v

.. code-block:: console

   aff3ct (Linux 64-bit, g++ 8.1, AVX2) v2.0.0-112-gc74ce62
   Copyright (c) 2016-2018 - MIT license.
   This is free software; see the source for copying conditions.  There is NO
   warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

.. _other-except-a2l:

``--except-a2l`` |image_advanced_argument|
""""""""""""""""""""""""""""""""""""""""""

**STRING**: |factory::Launcher::parameters::except-a2l|

Enhance the backtrace when displaying exception. This change the program
addresses into filenames and lines. It may take some seconds to do this work.

.. note:: This option works only on Unix based |OS| and if |AFF3CT| has been
   :ref:`compiled <compilation_compiler_options>` with debug symbols
   (``-g`` compile flag) and **without** ``NDEBUG`` macro (``-DNDEBUG`` flag).

.. _other-except-no-bt:

``--except-no-bt`` |image_advanced_argument|
""""""""""""""""""""""""""""""""""""""""""""

**STRING**: |factory::Launcher::parameters::except-no-bt|

Disable the print of backtrace when displaying exception.
