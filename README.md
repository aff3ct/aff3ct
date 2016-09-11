# How to run the code
This project use `cmake` in order to generate the Makefile.

## Install cmake
```bash
> sudo apt-get install cmake
```

## Generate the Makefile
```bash
> mkdir build
> cd build
> cmake ..
```

### Facultative cmake options

Enabling the GNU Scientific Librairy (GSL) (used for the AWGN channel):
```bash
> cmake .. -DENABLE_GSL=ON
```

Enabling the Intel Math Kernel Librairy (MKL) (used for the AWGN channel):
```bash
> cmake .. -DENABLE_MKL=ON
```

Enabling the SystemC/TLM standard between the modules:
```bash
> cmake .. -DENABLE_SYSTEMC=ON
```

### Modifying the CMakeCache.txt
By default the configuration file will generate a debug binary.
If we want to produce a release binary we have to modify the `CMAKE_BUILD_TYPE:STRING` variable like this:
```bash
CMAKE_BUILD_TYPE:STRING=Release
```
We can also add some flags in the `CMAKE_CXX_FLAGS:STRING` variable:
```bash
CMAKE_CXX_FLAGS:STRING=-Wall -march=native -funroll-loops
```

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
```bash
CMAKE_CXX_FLAGS:STRING=-Wall -march=native -funroll-loops -DENABLE_COOL_BASH
```

## Compiling with make
```bash
> make -j4
```
This command will use the generated Makefile.

## Run the code
Here are some examples of runs.

### Decoding of the Polar codes with the Successive Cancellation decoder

```bash
> ./bin/aff3ct --simu-type BFER --code-type POLAR -K 1024 -N 2048 --snr-min 1.5 --snr-max 3.01 --code-sigma 0.862 --dec-algo SC --dec-implem FAST
```

Expected output:
```bash
# -------------------------------------------------
# ---- A FAST FORWARD ERROR CORRECTION TOOL >> ----
# -------------------------------------------------
#
# Simulation parameters:
# * Simulation type                = BFER
# * Code type                      = POLAR codes
# * Number of information bits (K) = 1024
# * Codeword length            (N) = 2048
# * SNR min                        = 1.5 dB
# * SNR max                        = 3.01 dB
# * SNR step                       = 0.1 dB
# * Domain                         = LLR
# * Channel type                   = AWGN
# * Codewords generation method    = LCG
# * Type of bits               (B) = int (32 bits)
# * Type of reals              (R) = float (32 bits)
# * Max frame error count     (FE) = 100
# * Systematic encoding            = on
# * Decoding algorithm             = SC
# * Decoding implementation        = FAST
# * Multi-threading                = unused
# * Sigma for code generation      = 0.862000
# * Frozen bits generation method  = GA
#
(WW) The GA (Gaussian Approximation) frozen bits generator is unaccurate.
# The simulation is running...
# ------|----------|----------|----------|----------|----------||-------|-------|-------|---------|---------|----------||----------|----------
#   SNR |      FRA |       BE |       FE |      BER |      FER ||    LD |   DEC |    ST |    CTHR |    ITHR |  LATENCY || SIM_CTHR |    ET/RT 
#  (dB) |          |          |          |          |          ||   (%) |   (%) |   (%) |  (Mb/s) |  (Mb/s) |     (us) ||   (Mb/s) | (hhmmss) 
# ------|----------|----------|----------|----------|----------||-------|-------|-------|---------|---------|----------||----------|----------
   1.50 |      125 |    14628 |      100 | 1.14e-01 | 8.00e-01 ||   1.8 |  96.3 |   1.9 |   96.61 |   48.30 |    21.20 ||    17.55 | 00h00'00  
   1.60 |      177 |    14362 |      100 | 7.92e-02 | 5.65e-01 ||   1.8 |  96.3 |   1.9 |  108.17 |   54.09 |    18.93 ||    20.12 | 00h00'00  
   1.70 |      196 |    13419 |      100 | 6.69e-02 | 5.10e-01 ||   1.9 |  96.1 |   2.0 |  102.55 |   51.27 |    19.97 ||    19.07 | 00h00'00  
   1.80 |      243 |    13223 |      100 | 5.31e-02 | 4.12e-01 ||   1.9 |  96.1 |   2.0 |  106.07 |   53.03 |    19.31 ||    19.75 | 00h00'00  
   1.90 |      340 |    12734 |      100 | 3.66e-02 | 2.94e-01 ||   1.9 |  96.0 |   2.0 |  107.28 |   53.64 |    19.09 ||    20.31 | 00h00'00  
   2.00 |      466 |    12122 |      100 | 2.54e-02 | 2.15e-01 ||   1.9 |  96.1 |   2.0 |  109.98 |   54.99 |    18.62 ||    20.87 | 00h00'00  
   2.10 |      683 |    11794 |      100 | 1.69e-02 | 1.46e-01 ||   1.8 |  96.2 |   2.0 |  110.25 |   55.13 |    18.58 ||    21.16 | 00h00'00  
   2.20 |      907 |    10666 |      100 | 1.15e-02 | 1.10e-01 ||   1.9 |  96.2 |   2.0 |  111.56 |   55.78 |    18.36 ||    21.46 | 00h00'00  
   2.30 |     1674 |    11187 |      100 | 6.53e-03 | 5.97e-02 ||   1.8 |  96.2 |   2.0 |  113.38 |   56.69 |    18.06 ||    21.98 | 00h00'00  
   2.40 |     2464 |     9891 |      100 | 3.92e-03 | 4.06e-02 ||   1.8 |  96.2 |   2.0 |  113.22 |   56.61 |    18.09 ||    21.99 | 00h00'00  
   2.50 |     4428 |     9608 |      100 | 2.12e-03 | 2.26e-02 ||   1.8 |  96.2 |   2.0 |  114.37 |   57.19 |    17.91 ||    22.18 | 00h00'00  
   2.60 |     7846 |     9327 |      100 | 1.16e-03 | 1.27e-02 ||   1.9 |  96.1 |   2.0 |  113.79 |   56.89 |    18.00 ||    22.14 | 00h00'00  
   2.70 |    15081 |     8792 |      100 | 5.69e-04 | 6.63e-03 ||   1.9 |  96.1 |   2.0 |  114.38 |   57.19 |    17.91 ||    22.25 | 00h00'01  
   2.80 |    33648 |     8229 |      100 | 2.39e-04 | 2.97e-03 ||   1.9 |  96.1 |   2.0 |  114.47 |   57.23 |    17.89 ||    22.26 | 00h00'03  
   2.90 |    53292 |     7739 |      100 | 1.42e-04 | 1.88e-03 ||   1.9 |  96.1 |   2.0 |  114.38 |   57.19 |    17.91 ||    22.26 | 00h00'04  
   3.00 |   118658 |     8236 |      100 | 6.78e-05 | 8.43e-04 ||   1.9 |  96.1 |   2.0 |  114.43 |   57.22 |    17.90 ||    22.27 | 00h00'10  
# End of the simulation.
```

### Decoding of the Turbo codes with the LTE BCJR decoder (8 states trellis)

```bash
> ./bin/aff3ct --simu-type BFER --code-type TURBO -K 1024 -N 3072 --snr-min 0.0 --snr-max 1.01 --dec-algo LTE --dec-implem FAST --max-iter 6
```

Expected output:
```bash
# -------------------------------------------------
# ---- A FAST FORWARD ERROR CORRECTION TOOL >> ----
# -------------------------------------------------
#
# Simulation parameters:
# * Simulation type                = BFER
# * Code type                      = TURBO codes
# * Number of information bits (K) = 1024
# * Codeword length            (N) = 3072 + 12 (tail bits)
# * SNR min                        = 0 dB
# * SNR max                        = 1.01 dB
# * SNR step                       = 0.1 dB
# * Domain                         = LLR
# * Channel type                   = AWGN
# * Codewords generation method    = LCG
# * Type of bits               (B) = int (32 bits)
# * Type of reals              (R) = float (32 bits)
# * Max frame error count     (FE) = 100
# * Systematic encoding            = on
# * Decoding algorithm             = LTE
# * Decoding implementation        = FAST
# * Multi-threading                = unused
# * Decoding iterations per frame  = 6
# * Buffered encoding              = on
# * Interleaver                    = LTE
# * Scaling factor                 = LTE_VEC
#
# The simulation is running...
# ------|----------|----------|----------|----------|----------||-------|-------|-------|---------|---------|----------||----------|----------
#   SNR |      FRA |       BE |       FE |      BER |      FER ||    LD |   DEC |    ST |    CTHR |    ITHR |  LATENCY || SIM_CTHR |    ET/RT 
#  (dB) |          |          |          |          |          ||   (%) |   (%) |   (%) |  (Mb/s) |  (Mb/s) |     (us) ||   (Mb/s) | (hhmmss) 
# ------|----------|----------|----------|----------|----------||-------|-------|-------|---------|---------|----------||----------|----------
   0.00 |      110 |    13406 |      100 | 1.19e-01 | 9.09e-01 ||   0.7 |  99.2 |   0.2 |    9.58 |    3.19 |   320.53 ||     7.13 | 00h00'00  
   0.10 |      116 |    12227 |      100 | 1.03e-01 | 8.62e-01 ||   0.6 |  99.2 |   0.2 |   11.32 |    3.77 |   271.35 ||     8.52 | 00h00'00  
   0.20 |      133 |     9681 |      100 | 7.11e-02 | 7.52e-01 ||   0.7 |  99.1 |   0.2 |   11.22 |    3.74 |   273.80 ||     8.46 | 00h00'00  
   0.30 |      180 |     7538 |      100 | 4.09e-02 | 5.56e-01 ||   0.6 |  99.3 |   0.1 |   11.64 |    3.88 |   263.88 ||     8.78 | 00h00'00  
   0.40 |      208 |     6884 |      100 | 3.23e-02 | 4.81e-01 ||   0.6 |  99.3 |   0.1 |   11.56 |    3.85 |   265.73 ||     8.73 | 00h00'00  
   0.50 |      310 |     6114 |      100 | 1.93e-02 | 3.23e-01 ||   0.6 |  99.3 |   0.1 |   11.68 |    3.89 |   263.12 ||     8.82 | 00h00'00  
   0.60 |      651 |     4749 |      100 | 7.12e-03 | 1.54e-01 ||   0.6 |  99.3 |   0.1 |   11.68 |    3.89 |   263.02 ||     8.82 | 00h00'00  
   0.70 |     1267 |     5015 |      100 | 3.87e-03 | 7.89e-02 ||   0.6 |  99.3 |   0.1 |   11.71 |    3.90 |   262.30 ||     8.86 | 00h00'00  
   0.80 |     2911 |     3447 |      100 | 1.16e-03 | 3.44e-02 ||   0.6 |  99.3 |   0.1 |   11.72 |    3.91 |   262.04 ||     8.87 | 00h00'01  
   0.90 |     7844 |     2796 |      100 | 3.48e-04 | 1.27e-02 ||   0.6 |  99.3 |   0.1 |   11.74 |    3.91 |   261.75 ||     8.88 | 00h00'02  
   1.00 |    21695 |     2430 |      100 | 1.09e-04 | 4.61e-03 ||   0.6 |  99.3 |   0.1 |   11.55 |    3.85 |   265.99 ||     8.72 | 00h00'07  
# End of the simulation.
```

There is some additional examples of runs in the `scripts` folder (from the root folder).

## About eclipse
You may encounter an issue with eclipse, which doesn't handle C++11 as a default behaviour. To solve it you have to add the following:

- add `GXX_EXPERIMENTAL_CXX0X` (or `__GXX_EXPERIMENTAL_CXX0X__`) in Eclipse's symbols (C/C++ General > Path and Symbols > Symbols),
- add `-std=c++11` (or `-std=c++1y`) in Compiler Options (C/C++ General > Preprocessor Include Paths, Macros etc. > Providers  > DCT GCC Built-in Compiler Settings >  Commands to get compiler specs). You have to uncheck "Use global provider shared between projects".
