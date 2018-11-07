# AFF3CT Short Presentation

**AFF3CT** (A Fast Forward Error Correction Toolbox!) is a library dedicated to the [Forward Error Correction](https://en.wikipedia.org/wiki/Forward_error_correction) (FEC or channel coding).
It is written in **C++11** and it supports a large range of codes: from the well-spread **Turbo codes** to the very new **Polar codes** including the **Low-Density Parity-Check (LDPC) codes**.
AFF3CT can be used in two different ways, as:
- a dedicated **toolbox** or **library** for your projects, many modules (like codecs, modems, channels, ...) are available and easy to use,
- a **standalone application** for [Monte Carlo](https://en.wikipedia.org/wiki/Monte_Carlo_method) [BER/FER](https://en.wikipedia.org/wiki/Bit_error_rate) and [EXIT chart](https://en.wikipedia.org/wiki/EXIT_chart) simulations.

The following section focuses on the compilation and the execution of the AFF3CT standalone application.

# How to Compile and Run the Code

This project use `cmake` in order to generate any type of projects (Makefile, Visual Studio, Eclipse, CLion, etc.).

## Get the Git Submodules

AFF3CT depends on some other Git repositories (or submodules). It is highly recommended to get those submodules before trying to do anything else. Here is the command to get all the required submodules:

    $ git submodule update --init --recursive

## Example of a Makefile Project Generation (with the C++ GNU Compiler)

Open a terminal and type (from the `AFF3CT` root folder):

    $ mkdir build
    $ cd build
    $ cmake .. -G"Unix Makefiles" -DCMAKE_CXX_COMPILER=g++ -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-funroll-loops -march=native"

## Compile the Code with the Makefile

    $ make -j4

This command will use the generated Makefile.

## Run the Code
Here is an example of run. You can skip the computations of the current SNR point with the `ctrl+c` combination on the keyboard.
If you use `ctrl+c` twice in a small time-step (500ms), the program will stop.

### Decoding of the Polar Codes with the Successive Cancellation List Decoder (SCL)

Open a terminal and type (from the `$ROOT_AFF3CT/build/` folder):

    $ ./bin/aff3ct -C POLAR -m 1 -M 4 -s 0.25 -K 1755 -N 2048 --crc-poly 32-GZIP --dec-type ASCL

Expected output:

    # ----------------------------------------------------
    # ---- A FAST FORWARD ERROR CORRECTION TOOLBOX >> ----
    # ----------------------------------------------------
    # Parameters :
    # [...]
    #
    # The simulation is running...
    # ----------------------------------------------------------------------||---------------------
    #       Bit Error Rate (BER) and Frame Error Rate (FER) depending       ||  Global throughput
    #                    on the Signal Noise Ratio (SNR)                    ||  and elapsed time
    # ----------------------------------------------------------------------||---------------------
    # -------|-------|----------|----------|----------|----------|----------||----------|----------
    #  Es/N0 | Eb/N0 |      FRA |       BE |       FE |      BER |      FER ||  SIM_THR |    ET/RT
    #   (dB) |  (dB) |          |          |          |          |          ||   (Mb/s) | (hhmmss)
    # -------|-------|----------|----------|----------|----------|----------||----------|----------
        0.25 |  1.00 |      103 |    15522 |      103 | 8.75e-02 | 1.00e+00 ||     2.37 | 00h00'00
        0.50 |  1.25 |      103 |    14705 |      103 | 8.29e-02 | 1.00e+00 ||     5.04 | 00h00'00
        0.75 |  1.50 |      103 |    13801 |      103 | 7.78e-02 | 1.00e+00 ||     5.26 | 00h00'00
        1.00 |  1.75 |      103 |    12988 |      103 | 7.32e-02 | 1.00e+00 ||     5.22 | 00h00'00
        1.25 |  2.00 |      103 |    12072 |      103 | 6.80e-02 | 1.00e+00 ||     5.20 | 00h00'00
        1.50 |  2.25 |      103 |    10801 |      103 | 6.09e-02 | 1.00e+00 ||     5.12 | 00h00'00
        1.75 |  2.50 |      106 |     8266 |      103 | 4.53e-02 | 9.72e-01 ||     5.23 | 00h00'00
        2.00 |  2.75 |      138 |     6789 |      102 | 2.86e-02 | 7.39e-01 ||     6.09 | 00h00'00
        2.25 |  3.00 |      182 |     4777 |      103 | 1.52e-02 | 5.66e-01 ||     7.12 | 00h00'00
        2.50 |  3.25 |      497 |     3751 |      102 | 4.38e-03 | 2.05e-01 ||    11.76 | 00h00'00
        2.75 |  3.50 |     2445 |     2723 |      100 | 6.46e-04 | 4.09e-02 ||    19.59 | 00h00'00
        3.00 |  3.75 |    18817 |     2552 |      100 | 7.87e-05 | 5.31e-03 ||    26.25 | 00h00'01
        3.25 |  4.00 |   174249 |     1910 |      100 | 6.36e-06 | 5.74e-04 ||    28.97 | 00h00'10
    # End of the simulation.

# More

A list of the AFF3CT related web pages:
- [Official wiki](https://github.com/aff3ct/aff3ct/wiki)
- [Official website](https://aff3ct.github.io/)
- [Scientific publications](https://aff3ct.github.io/publications.html)
- [Contributing guidelines](https://github.com/aff3ct/aff3ct/blob/master/CONTRIBUTING.md)
