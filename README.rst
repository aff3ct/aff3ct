AFF3CT: A Fast Forward Error Correction Toolbox!
================================================

**AFF3CT** is a simulator dedicated to the Forward Error Correction (FEC or
**channel coding**). It is written in **C++** and it supports a large range of
codes: from the well-spread **Turbo codes** to the very new **Polar codes**
including the **Low-Density Parity-Check (LDPC) codes**. **AFF3CT** is a command
line program and it simulates communication chains based on a Monte Carlo
method.

It is very easy to use, for instance, to estimate the BER/FER decoding
performances of the (2048,1723) Polar code from 1.0 to 4.0 dB:

.. code-block:: bash

   aff3ct -C "POLAR" -K 1723 -N 2048 -m 1.0 -M 4.0

And the output will be:

.. code-block:: bash

   # ----------------------------------------------------
   # ---- A FAST FORWARD ERROR CORRECTION TOOLBOX >> ----
   # ----------------------------------------------------
   # Parameters :
   # [...]
   #
   # The simulation is running...
   # ---------------------||------------------------------------------------------||---------------------
   #  Signal Noise Ratio  ||   Bit Error Rate (BER) and Frame Error Rate (FER)    ||  Global throughput
   #         (SNR)        ||                                                      ||  and elapsed time
   # ---------------------||------------------------------------------------------||---------------------
   # ----------|----------||----------|----------|----------|----------|----------||----------|----------
   #     Es/N0 |    Eb/N0 ||      FRA |       BE |       FE |      BER |      FER ||  SIM_THR |    ET/RT
   #      (dB) |     (dB) ||          |          |          |          |          ||   (Mb/s) | (hhmmss)
   # ----------|----------||----------|----------|----------|----------|----------||----------|----------
          0.25 |     1.00 ||      104 |    16425 |      104 | 9.17e-02 | 1.00e+00 ||    4.995 | 00h00'00
          1.25 |     2.00 ||      104 |    12285 |      104 | 6.86e-02 | 1.00e+00 ||   13.678 | 00h00'00
          2.25 |     3.00 ||      147 |     5600 |      102 | 2.21e-02 | 6.94e-01 ||   14.301 | 00h00'00
          3.25 |     4.00 ||     5055 |     2769 |      100 | 3.18e-04 | 1.98e-02 ||   30.382 | 00h00'00
   # End of the simulation.

Features
--------

**The simulator targets high speed simulations** and extensively uses parallel
techniques like SIMD, multi-threading and multi-nodes programming models.
Below, a list of the features that motivated the creation of the simulator:

   #. **reproduce state-of-the-art decoding performances**,
   #. **explore various channel code configurations**, find new trade-offs,
   #. **prototype hardware implementation** (fixed-point receivers, hardware in
      the loop tools),
   #. **reuse tried and tested modules** and add yours,
   #. **alternative to MATLAB**, if you seek to reduce simulations
      time.

Installation
------------

First make sure to have installed a C++11 compiler, CMake and Git. Then install
AFF3CT by running:

.. code-block:: bash

   git clone --recursive https://github.com/aff3ct/aff3ct.git
   mkdir aff3ct/build
   cd aff3ct/build
   cmake .. -DCMAKE_BUILD_TYPE="Release"
   make -j4

Contribute
----------

- Source Code: https://github.com/aff3ct/aff3ct
- Contributing guidelines: https://github.com/aff3ct/aff3ct/blob/master/CONTRIBUTING.rst

Support
-------

If you are having issues, please let us know.
We have an issue tracker at: https://github.com/aff3ct/aff3ct/issues

License
-------

The project is licensed under the MIT license.

External Links
--------------

- Official website: https://aff3ct.github.io
- Documentation: https://aff3ct.readthedocs.io

