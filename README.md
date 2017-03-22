# How to compile and run the code
This project use `cmake` in order to generate any type of projects (Makefile, Visual Studio projects, Eclipse projects, etc.).

## Install cmake

### On Linux/Debian-like

Open a shell and type:

    $ sudo apt-get install cmake

### On Windows

Download and install `cmake`: [https://cmake.org/files/v3.7/cmake-3.7.1-win64-x64.msi](https://cmake.org/files/v3.7/cmake-3.7.1-win64-x64.msi).

Make sure to add the `cmake` program to the `PATH`.

## Exemple of a Makefile project generation (with the C++ GNU compiler)

Open a shell and type (from the `AFF3CT` root folder):

    $ mkdir build
    $ cd build
    $ cmake .. -G"Unix Makefiles" -DCMAKE_CXX_COMPILER=g++ -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-Wall -funroll-loops -march=native -DMULTI_PREC" -DCMAKE_CXX_FLAGS_RELEASE="-Ofast"

## Compile the code with the Makefile

    $ make -j4

This command will use the generated Makefile.

## Run the code
Here are some examples of runs. You can skip the computations of the current SNR point with the `ctrl+C` combination on the keyboard.
If you use `ctrl+C` twice in a small time-step (500ms), the program will stop.

### Decoding of the Polar codes with the Successive Cancellation decoder

Open a shell and type (from the `$ROOT_AFF3CT/build/` folder):

    $ ./bin/aff3ct --sim-type BFER --cde-type POLAR -K 1024 -N 2048 -m 1.5 -M 2.81 --cde-fb-gen-method GA -t 1

Expected output:

    # -------------------------------------------------
    # ---- A FAST FORWARD ERROR CORRECTION TOOL >> ----
    # -------------------------------------------------
    # Parameters:
    # * Simulation ------------------------------------
    #    ** Type                    = BFER
    #    ** SNR min (m)             = 1.500000 dB
    #    ** SNR max (M)             = 2.810000 dB
    #    ** SNR step (s)            = 0.100000 dB
    #    ** Type of bits            = int (32 bits)
    #    ** Type of reals           = float (32 bits)
    #    ** Inter frame level       = 1
    #    ** Seed                    = 0
    #    ** Multi-threading (t)     = 1 thread(s)
    # * Code ------------------------------------------
    #    ** Type                    = POLAR
    #    ** Info. bits (K)          = 1024
    #    ** Codeword size (N)       = 2048
    #    ** Coset approach (c)      = off
    #    ** Sigma for code gen.     = adaptative
    #    ** Frozen bits gen. method = GA
    # * Source ----------------------------------------
    #    ** Type                    = RAND
    # * Encoder ---------------------------------------
    #    ** Type                    = POLAR
    #    ** Systematic encoding     = on
    # * Modulator -------------------------------------
    #    ** Type                    = BPSK
    #    ** Bits per symbol         = 1
    #    ** Sampling factor         = 1
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
    # The simulation is running...
    # -------------------------------------------------------------||--------------------------------||---------------------
    #   Bit Error Rate (BER) and Frame Error Rate (FER) depending  ||     Decoder throughput and     ||  Global throughput  
    #             on the Signal Noise Ratio (SNR) Eb/N0            ||      latency (per thread)      ||  and elapsed time   
    # -------------------------------------------------------------||--------------------------------||---------------------
    # ------|----------|----------|----------|----------|----------||----------|----------|----------||----------|----------
    #   SNR |      FRA |       BE |       FE |      BER |      FER ||     CTHR |     ITHR |  LATENCY || SIM_CTHR |    ET/RT 
    #  (dB) |          |          |          |          |          ||   (Mb/s) |   (Mb/s) |     (us) ||   (Mb/s) | (hhmmss) 
    # ------|----------|----------|----------|----------|----------||----------|----------|----------||----------|----------
       1.50 |      397 |     6512 |      100 | 1.60e-02 | 2.52e-01 ||    97.92 |    48.96 |    20.91 ||    13.19 | 00h00'00  
       1.60 |      522 |     6100 |      100 | 1.14e-02 | 1.92e-01 ||   119.57 |    59.79 |    17.13 ||    17.35 | 00h00'00  
       1.70 |      695 |     6232 |      100 | 8.76e-03 | 1.44e-01 ||   118.62 |    59.31 |    17.27 ||    17.42 | 00h00'00  
       1.80 |      988 |     5160 |      100 | 5.10e-03 | 1.01e-01 ||   119.23 |    59.62 |    17.18 ||    17.53 | 00h00'00  
       1.90 |     1540 |     4590 |      100 | 2.91e-03 | 6.49e-02 ||   117.27 |    58.63 |    17.46 ||    17.78 | 00h00'00  
       2.00 |     2593 |     4880 |      100 | 1.84e-03 | 3.86e-02 ||   117.29 |    58.64 |    17.46 ||    17.96 | 00h00'00  
       2.10 |     3519 |     4488 |      100 | 1.25e-03 | 2.84e-02 ||   117.14 |    58.57 |    17.48 ||    18.02 | 00h00'00  
       2.20 |     5374 |     4636 |      100 | 8.42e-04 | 1.86e-02 ||   115.91 |    57.95 |    17.67 ||    18.02 | 00h00'00  
       2.30 |    11013 |     3728 |      100 | 3.31e-04 | 9.08e-03 ||   115.70 |    57.85 |    17.70 ||    18.02 | 00h00'01  
       2.40 |    20949 |     3812 |      100 | 1.78e-04 | 4.77e-03 ||   114.89 |    57.45 |    17.83 ||    17.92 | 00h00'02  
       2.50 |    30454 |     3226 |      100 | 1.03e-04 | 3.28e-03 ||   115.22 |    57.61 |    17.77 ||    18.01 | 00h00'03  
       2.60 |    40835 |     3990 |      100 | 9.54e-05 | 2.45e-03 ||   113.05 |    56.52 |    18.12 ||    17.93 | 00h00'04  
       2.70 |    93116 |     3216 |      100 | 3.37e-05 | 1.07e-03 ||   111.32 |    55.66 |    18.40 ||    17.73 | 00h00'10  
       2.80 |   199253 |     2795 |      100 | 1.37e-05 | 5.02e-04 ||   109.16 |    54.58 |    18.76 ||    17.82 | 00h00'22  
    # End of the simulation.

## More

See the [AFF3CT Wiki](https://github.com/aff3ct/aff3ct/wiki).