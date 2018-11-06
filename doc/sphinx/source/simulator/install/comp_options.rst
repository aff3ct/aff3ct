Compilation options
-------------------

Facultative cmake options
^^^^^^^^^^^^^^^^^^^^^^^^^

Enabling the ``GNU Scientific`` Library (GSL) (used for the AWGN channel):

.. code-block:: bash

   cmake .. -DENABLE_GSL=ON

Enabling the ``Intel Math Kernel`` Library (MKL) (used for the AWGN channel):

.. code-block:: bash

   cmake .. -DENABLE_MKL=ON

Enabling the ``SystemC/TLM`` standard between the modules:

.. code-block:: bash

   cmake .. -DENABLE_SYSTEMC=ON

Enabling ``MPI`` (simulations over multiple nodes/computers) for the *BFER* and
*BFERI* simulations:

.. code-block:: bash

   cmake .. -DENABLE_MPI=ON


Modifying the CMakeCache.txt
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

By default the configuration file will generate a debug binary.If we want to
produce a release binary we have to modify the **CMAKE_BUILD_TYPE:STRING**
variable like this::

   CMAKE_BUILD_TYPE:STRING=Release

We can also add some flags in the **CMAKE_CXX_FLAGS:STRING** variable::

   CMAKE_CXX_FLAGS:STRING="-Wall -march=native -funroll-loops"

Precision options
"""""""""""""""""

There is some specific flags to configure the type of data to use in the
simulation modules (you have to add those flags in the last above variable):

   * ``-DPREC_64_BIT``: use 64-bit precision when it is possible (floating-point representation for the LLRs),
   * ``-DPREC_32_BIT``: use 32-bit precision when it is possible (floating-point representation for the LLRs, this is the default when you don't specify any flag),
   * ``-DPREC_16_BIT``: use 16-bit precision when it is possble (fixed-point representation for the LLRs),
   * ``-DPREC_8_BIT``: use 8-bit precision when it is possible (fixed-point representation for the LLRs),
   * ``-DMULTI_PREC``: build a binary containing all the previous precisions (specific type can be selected at runtime with the -p option).


CPU Architecture specifications
"""""""""""""""""""""""""""""""

Notice that some parts of the code use **SIMD** (Single Instruction Multiple Data)
parallelism and this type of instructions often requires additionnal compiler
options to be enabled (specific GNU compiler flags are given):

   * ``-msse2``: enable the SSE2 set of instructions on x86 CPUs (128-bit vector size, required for 32-bit and 64-bit data),
   * ``-mssse3``: enable the SSSE3 set of instructions on x86 CPUs (128-bit vector size, specifically required for 32-bit data and the SC_FAST decoder),
   * ``-msse4.1``: enable the SSE4.1 set of instructions on x86 CPUs (128-bit vector size, required for 8-bit and 16-bit data),
   * ``-mavx``: enable the AVX set of instructions on x86 CPUs (256-bit vector size, required for 32-bit and 64-bit data),
   * ``-mavx2``: enable the AVX2 set of instructions on x86 CPUs (256-bit vector size, required for 8-bit and 16-bit data),
   * ``-mfpu=neon``: enable the NEON set of instructions on ARMv7 and ARMv8 CPUs (128-bit vector size, required for 8-bit, 16-bit data and 32-bit data),
   * ``-march=native``: let the compiler choose the best set of instructions available on the current architecture (it does not work for ARMv7 architectures since the NEON instructions are not IEEE 754 compatible).


Enabling the cool bash mode
"""""""""""""""""""""""""""

Enable the colors and boldfaces in the shell with the ``-DENABLE_COOL_BASH`` flag.