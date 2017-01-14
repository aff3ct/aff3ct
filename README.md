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

    $ ./bin/aff3ct --sim-type BFER --cde-type POLAR -K 1024 -N 2048 -m 1.5 -M 3.01 --cde-fb-gen-method GA --cde-sigma 0.862 --dec-type SC --dec-implem FAST -t 1

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

## More

See the [AFF3CT Wiki](https://github.com/aff3ct/aff3ct/wiki).