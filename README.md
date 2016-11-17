# How to compile and run the code
This project use `cmake` in order to generate the Makefile.

## Install cmake

    $ sudo apt-get install cmake

## Generate the Makefile

    $ mkdir build
    $ cd build
    $ cmake ..

### Facultative cmake options

Enabling the GNU Scientific Librairy (GSL) (used for the AWGN channel):

    $ cmake .. -DENABLE_GSL=ON

Enabling the Intel Math Kernel Librairy (MKL) (used for the AWGN channel):

    $ cmake .. -DENABLE_MKL=ON

Enabling the SystemC/TLM standard between the modules:

    $ cmake .. -DENABLE_SYSTEMC=ON

### Modifying the CMakeCache.txt
By default the configuration file will generate a debug binary.
If we want to produce a release binary we have to modify the `CMAKE_BUILD_TYPE:STRING` variable like this:

    CMAKE_BUILD_TYPE:STRING=Release

We can also add some flags in the `CMAKE_CXX_FLAGS:STRING` variable:

    CMAKE_CXX_FLAGS:STRING=-Wall -march=native -funroll-loops

There is some specific flags to configure the type of data to use in the simulation modules (you have to add those flags in the `CMAKE_CXX_FLAGS:STRING` variable):

- `-DPREC_64_BIT`: use 64-bit precision when it is possible (floating-point representation for the LLRs),
- `-DPREC_32_BIT`: use 32-bit precision when it is possible (floating-point representation for the LLRs, this is the default when you don't specify any flag),
- `-DPREC_16_BIT`: use 16-bit precision when it is possble (fixed-point representation for the LLRs),
- `-DPREC_8_BIT`: use 8-bit precision  when it is possible (fixed-point representation for the LLRs),
- `-DMULTI_PREC`: build a binary containing all the previous precisions (specific type can be selected at runtime with the `--prec` option).

Notice that some parts of the code use SIMD (Single Instruction Multiple Data) parallelism and this type of instructions often requires additionnal compiler options to be enabled (specific GNU compiler flags are given):

- `-msse2`: enable the SSE2 set of instructions on x86 CPUs (128-bit vector size, required for 32-bit and 64-bit data),
- `-mssse3`: enable the SSSE3 set of instructions on x86 CPUs (128-bit vector size, specifically required for 32-bit data and the `SC_FAST` decoder),
- `-msse4.1`: enable the SSE4.1 set of instructions on x86 CPUs (128-bit vector size, required for 8-bit and 16-bit data),
- `-mavx`: enable the AVX set of instructions on x86 CPUs (256-bit vector size, required for 32-bit and 64-bit data),
- `-mavx2`: enable the AVX2 set of instructions on x86 CPUs (256-bit vector size, required for 8-bit and 16-bit data),
- `-mfpu=neon`: enable the NEON set of instructions on ARMv7 and ARMv8 CPUs (128-bit vector size, required for 8-bit, 16-bit data and 32-bit data),
- `-march=native`: let the compiler choose the best set of instructions available on the current architecture (it does not work for ARMv7 architectures since the NEON instructions are not IEEE 754 compatible).

#### Enabling the cool bash mode

Enable the colors and boldfaces in the shell with the `-DENABLE_COOL_BASH` flag:

    CMAKE_CXX_FLAGS:STRING=-Wall -march=native -funroll-loops -DENABLE_COOL_BASH

## Compiling with make

    $ make -j4

This command will use the generated Makefile.

## Run the code
Here are some examples of runs. You can skip the computations of the current SNR point with the `ctrl+C` combination on the keyboard.
If you use `ctrl+C` twice in a small time-step (500ms), the program will stop.

### Decoding of the Polar codes with the Successive Cancellation decoder

    $ ./bin/aff3ct --sim-type BFER --cde-type POLAR -K 1024 -N 2048 -m 1.5 -M 3.01 --cde-fb-gen-method GA --cde-sigma 0.862 --dec-type SC --dec-implem FAST

Expected output:

    # -------------------------------------------------
    # ---- A FAST FORWARD ERROR CORRECTION TOOL >> ----
    # -------------------------------------------------
    # Parameters:
    # * Simulation ------------------------------------
    #    ** Type                    = BFER
    #    ** SNR min (m)             = 1.500000 dB
    #    ** SNR max (M)             = 3.010000 dB
    #    ** SNR step (s)            = 0.100000 dB
    #    ** Type of bits            = int (32 bits)
    #    ** Type of reals           = float (32 bits)
    #    ** Multi-threading (t)     = 1 thread(s)
    # * Code ------------------------------------------
    #    ** Type                    = POLAR codes
    #    ** Info. bits (K)          = 1024
    #    ** Codeword size (N)       = 2048
    #    ** Sigma for code gen.     = 0.862000
    #    ** Frozen bits gen. method = GA
    # * Source ----------------------------------------
    #    ** Type                    = RAND
    # * Encoder ---------------------------------------
    #    ** Systematic encoding     = on
    # * Modulator -------------------------------------
    #    ** Type                    = BPSK
    # * Channel ---------------------------------------
    #    ** Type                    = AWGN
    #    ** Domain                  = LLR
    # * Demodulator -----------------------------------
    #    ** Sigma square            = on
    #    ** Max type                = unused
    # * Decoder ---------------------------------------
    #    ** Type (D)                = SC
    #    ** Implementation          = FAST
    # * Monitor ---------------------------------------
    #    ** Frame error count (e)   = 100
    #
    (WW) The GA (Gaussian Approximation) frozen bits generator is inaccurate.
    # The simulation is running...
    # -------------------------------------------------------------||--------------------------------||---------------------
    #   Bit Error Rate (BER) and Frame Error Rate (FER) depending  ||     Decoder throughput and     ||  Global throughput  
    #             on the Signal Noise Ratio (SNR) Eb/N0            ||      latency (per thread)      ||  and elapsed time   
    # -------------------------------------------------------------||--------------------------------||---------------------
    # ------|----------|----------|----------|----------|----------||----------|----------|----------||----------|----------
    #   SNR |      FRA |       BE |       FE |      BER |      FER ||     CTHR |     ITHR |  LATENCY || SIM_CTHR |    ET/RT 
    #  (dB) |          |          |          |          |          ||   (Mb/s) |   (Mb/s) |     (us) ||   (Mb/s) | (hhmmss) 
    # ------|----------|----------|----------|----------|----------||----------|----------|----------||----------|----------
       1.50 |      137 |    14915 |      100 | 1.06e-01 | 7.30e-01 ||    84.68 |    42.34 |    24.19 ||    13.92 | 00h00'00 
       1.60 |      173 |    14517 |      100 | 8.19e-02 | 5.78e-01 ||    91.34 |    45.67 |    22.42 ||    15.42 | 00h00'00 
       1.70 |      200 |    13867 |      100 | 6.77e-02 | 5.00e-01 ||    86.87 |    43.43 |    23.58 ||    14.76 | 00h00'00 
       1.80 |      232 |    13271 |      100 | 5.59e-02 | 4.31e-01 ||    89.39 |    44.70 |    22.91 ||    15.56 | 00h00'00 
       1.90 |      320 |    12323 |      100 | 3.76e-02 | 3.12e-01 ||    91.01 |    45.50 |    22.50 ||    15.94 | 00h00'00 
       2.00 |      449 |    11957 |      100 | 2.60e-02 | 2.23e-01 ||    94.18 |    47.09 |    21.74 ||    16.54 | 00h00'00 
       2.10 |      627 |    11253 |      100 | 1.75e-02 | 1.59e-01 ||    93.19 |    46.60 |    21.98 ||    16.45 | 00h00'00 
       2.20 |      965 |    11123 |      100 | 1.13e-02 | 1.04e-01 ||    94.02 |    47.01 |    21.78 ||    16.71 | 00h00'00 
       2.30 |     1644 |    10204 |      100 | 6.06e-03 | 6.08e-02 ||    94.50 |    47.25 |    21.67 ||    16.75 | 00h00'00 
       2.40 |     2634 |     9371 |      100 | 3.47e-03 | 3.80e-02 ||    94.76 |    47.38 |    21.61 ||    16.84 | 00h00'00 
       2.50 |     3634 |     9491 |      100 | 2.55e-03 | 2.75e-02 ||    94.54 |    47.27 |    21.66 ||    16.79 | 00h00'00 
       2.60 |     6227 |     9152 |      100 | 1.44e-03 | 1.61e-02 ||    86.86 |    43.43 |    23.58 ||    15.64 | 00h00'00 
       2.70 |    11502 |     8602 |      100 | 7.30e-04 | 8.69e-03 ||    82.97 |    41.48 |    24.68 ||    15.07 | 00h00'01 
       2.80 |    24300 |     8359 |      100 | 3.36e-04 | 4.12e-03 ||    94.45 |    47.22 |    21.68 ||    16.86 | 00h00'02 
       2.90 |    58746 |     7961 |      100 | 1.32e-04 | 1.70e-03 ||    92.75 |    46.37 |    22.08 ||    16.55 | 00h00'07 
       3.00 |   130491 |     8150 |      100 | 6.10e-05 | 7.66e-04 ||    93.19 |    46.60 |    21.98 ||    16.65 | 00h00'16 
    # End of the simulation.

### Decoding of the Turbo codes with the LTE BCJR decoder (8 states trellis)

    $ ./bin/aff3ct --sim-type BFER --cde-type TURBO -K 1024 -N 3072 -m 0.0 -M 1.01 --dec-type LTE --dec-implem FAST -i 6

Expected output:

    # -------------------------------------------------
    # ---- A FAST FORWARD ERROR CORRECTION TOOL >> ----
    # -------------------------------------------------
    # Parameters:
    # * Simulation ------------------------------------
    #    ** Type                   = BFER
    #    ** SNR min (m)            = 0.000000 dB
    #    ** SNR max (M)            = 1.010000 dB
    #    ** SNR step (s)           = 0.100000 dB
    #    ** Type of bits           = int (32 bits)
    #    ** Type of reals          = float (32 bits)
    #    ** Multi-threading (t)    = 1 thread(s)
    # * Code ------------------------------------------
    #    ** Type                   = TURBO codes
    #    ** Info. bits (K)         = 1024
    #    ** Codeword size (N)      = 3072 + 12 (tail bits)
    # * Source ----------------------------------------
    #    ** Type                   = RAND
    # * Encoder ---------------------------------------
    #    ** Systematic encoding    = on
    #    ** Buffered               = on
    # * Interleaver -----------------------------------
    #    ** Type                   = LTE
    # * Modulator -------------------------------------
    #    ** Type                   = BPSK
    # * Channel ---------------------------------------
    #    ** Type                   = AWGN
    #    ** Domain                 = LLR
    # * Demodulator -----------------------------------
    #    ** Sigma square           = on
    #    ** Max type               = unused
    # * Decoder ---------------------------------------
    #    ** Type (D)               = LTE
    #    ** Implementation         = FAST
    #    ** Num. of iterations (i) = 6
    #    ** Scaling factor         = LTE_VEC
    #    ** Max type               = MAX
    # * Monitor ---------------------------------------
    #    ** Frame error count (e)  = 100
    #
    # The simulation is running...
    # -------------------------------------------------------------||--------------------------------||---------------------
    #   Bit Error Rate (BER) and Frame Error Rate (FER) depending  ||     Decoder throughput and     ||  Global throughput  
    #             on the Signal Noise Ratio (SNR) Eb/N0            ||      latency (per thread)      ||  and elapsed time   
    # -------------------------------------------------------------||--------------------------------||---------------------
    # ------|----------|----------|----------|----------|----------||----------|----------|----------||----------|----------
    #   SNR |      FRA |       BE |       FE |      BER |      FER ||     CTHR |     ITHR |  LATENCY || SIM_CTHR |    ET/RT 
    #  (dB) |          |          |          |          |          ||   (Mb/s) |   (Mb/s) |     (us) ||   (Mb/s) | (hhmmss) 
    # ------|----------|----------|----------|----------|----------||----------|----------|----------||----------|----------
       0.00 |      104 |    12658 |      100 | 1.19e-01 | 9.62e-01 ||    10.29 |     3.43 |   298.67 ||     7.04 | 00h00'00 
       0.10 |      111 |    10405 |      100 | 9.15e-02 | 9.01e-01 ||    10.77 |     3.59 |   285.19 ||     7.41 | 00h00'00 
       0.20 |      131 |     9061 |      100 | 6.75e-02 | 7.63e-01 ||    10.75 |     3.58 |   285.68 ||     7.39 | 00h00'00 
       0.30 |      167 |     7703 |      100 | 4.50e-02 | 5.99e-01 ||    11.25 |     3.75 |   273.09 ||     7.85 | 00h00'00 
       0.40 |      216 |     5542 |      100 | 2.51e-02 | 4.63e-01 ||    11.22 |     3.74 |   273.80 ||     7.84 | 00h00'00 
       0.50 |      356 |     5092 |      100 | 1.40e-02 | 2.81e-01 ||    11.37 |     3.79 |   270.09 ||     7.96 | 00h00'00 
       0.60 |      656 |     4600 |      100 | 6.85e-03 | 1.52e-01 ||    11.31 |     3.77 |   271.52 ||     7.95 | 00h00'00 
       0.70 |     1246 |     3987 |      100 | 3.12e-03 | 8.03e-02 ||    11.41 |     3.80 |   269.32 ||     8.02 | 00h00'00 
       0.80 |     2738 |     3562 |      100 | 1.27e-03 | 3.65e-02 ||    11.40 |     3.80 |   269.53 ||     8.01 | 00h00'01 
       0.90 |     8240 |     2935 |      100 | 3.48e-04 | 1.21e-02 ||    11.39 |     3.80 |   269.79 ||     8.00 | 00h00'03 
       1.00 |    28349 |     2717 |      100 | 9.36e-05 | 3.53e-03 ||    11.23 |     3.74 |   273.53 ||     7.88 | 00h00'11 
    # End of the simulation.

There is some additional examples of runs in the `scripts` folder (from the root folder).

## About eclipse
You may encounter an issue with eclipse, which doesn't handle C++11 as a default behaviour. To solve it you have to add the following:

- add `GXX_EXPERIMENTAL_CXX0X` (or `__GXX_EXPERIMENTAL_CXX0X__`) in Eclipse's symbols (C/C++ General > Path and Symbols > Symbols),
- add `-std=c++11` (or `-std=c++1y`) in Compiler Options (C/C++ General > Preprocessor Include Paths, Macros etc. > Providers  > DCT GCC Built-in Compiler Settings >  Commands to get compiler specs). You have to uncheck "Use global provider shared between projects".
