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
    $ cmake .. -G"Unix Makefiles" -DCMAKE_CXX_COMPILER=g++ -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-funroll-loops -march=native -DMULTI_PREC"

## Compile the code with the Makefile

    $ make -j4

This command will use the generated Makefile.

## Run the code
Here is an example of run. You can skip the computations of the current SNR point with the `ctrl+C` combination on the keyboard.
If you use `ctrl+C` twice in a small time-step (500ms), the program will stop.

### Decoding of the Polar codes with the Successive Cancellation List decoder (SCL)

Open a shell and type (from the `$ROOT_AFF3CT/build/` folder):

    $ ./bin/aff3ct --sim-type BFER --cde-type POLAR -m 1 -M 4 -s 0.25 -K 1755 -N 2048 --crc-poly 32-GZIP --dec-type ASCL

Expected output:

    # -------------------------------------------------
    # ---- A FAST FORWARD ERROR CORRECTION TOOL >> ----
    # -------------------------------------------------
    # Parameters:
    # * Simulation ------------------------------------
    #    ** Type                     = BFER
    #    ** SNR min (m)              = 1.000000 dB
    #    ** SNR max (M)              = 4.000100 dB
    #    ** SNR step (s)             = 0.250000 dB
    #    ** Type of bits             = int (32 bits)
    #    ** Type of reals            = float (32 bits)
    #    ** Inter frame level        = 1
    #    ** Seed                     = 0
    #    ** Multi-threading (t)      = 4 thread(s)
    # * Code ------------------------------------------
    #    ** Type                     = POLAR
    #    ** Info. bits (K)           = 1723 + 32 (CRC)
    #    ** Codeword size (N)        = 2048
    #    ** Coset approach (c)       = off
    #    ** Sigma for code gen.      = adaptative
    #    ** Frozen bits gen. method  = GA
    # * Source ----------------------------------------
    #    ** Type                     = RAND
    # * CRC -------------------------------------------
    #    ** Type                     = FAST
    #    ** Name                     = 32-GZIP
    #    ** Polynomial (hexadecimal) = 0x4c11db7
    #    ** Size (in bit)            = 32
    #    ** Add CRC in the code rate = off
    # * Encoder ---------------------------------------
    #    ** Type                     = POLAR
    #    ** Systematic encoding      = on
    # * Modulator -------------------------------------
    #    ** Type                     = BPSK
    #    ** Bits per symbol          = 1
    #    ** Sampling factor          = 1
    # * Channel ---------------------------------------
    #    ** Type                     = AWGN
    #    ** Domain                   = LLR
    # * Demodulator -----------------------------------
    #    ** Sigma square             = on
    #    ** Max type                 = unused
    # * Decoder ---------------------------------------
    #    ** Type (D)                 = ASCL
    #    ** Implementation           = FAST
    #    ** Max num. of lists (L)    = 8
    #    ** Adaptative mode          = full
    #    ** Polar node types         = {R0,R0L,R1,REP,REPL,SPC}
    # * Monitor ---------------------------------------
    #    ** Frame error count (e)    = 100
    #    ** Bad frames tracking      = off
    #    ** Bad frames replay        = off
    #
    # The simulation is running...
    # ----------------------------------------------------------------------||--------------------------------||---------------------
    #       Bit Error Rate (BER) and Frame Error Rate (FER) depending       ||     Decoder throughput and     ||  Global throughput  
    #                    on the Signal Noise Ratio (SNR)                    ||      latency (per thread)      ||  and elapsed time   
    # ----------------------------------------------------------------------||--------------------------------||---------------------
    # -------|-------|----------|----------|----------|----------|----------||----------|----------|----------||----------|----------
    #  Es/N0 | Eb/N0 |      FRA |       BE |       FE |      BER |      FER ||     CTHR |     ITHR |  LATENCY || SIM_CTHR |    ET/RT 
    #   (dB) |  (dB) |          |          |          |          |          ||   (Mb/s) |   (Mb/s) |     (us) ||   (Mb/s) | (hhmmss) 
    # -------|-------|----------|----------|----------|----------|----------||----------|----------|----------||----------|----------
        0.25 |  1.00 |      103 |    15777 |      103 | 8.73e-02 | 1.00e+00 ||     2.46 |     2.10 |   834.12 ||     5.23 | 00h00'00  
        0.50 |  1.25 |      103 |    15096 |      103 | 8.35e-02 | 1.00e+00 ||     2.72 |     2.33 |   752.64 ||     6.72 | 00h00'00  
        0.75 |  1.50 |      103 |    13921 |      103 | 7.70e-02 | 1.00e+00 ||     3.07 |     2.63 |   666.28 ||     7.36 | 00h00'00  
        1.00 |  1.75 |      103 |    13156 |      103 | 7.28e-02 | 1.00e+00 ||     3.22 |     2.76 |   636.18 ||     6.62 | 00h00'00  
        1.25 |  2.00 |      103 |    12146 |      103 | 6.72e-02 | 1.00e+00 ||     3.12 |     2.68 |   655.87 ||     6.94 | 00h00'00  
        1.50 |  2.25 |      103 |    10499 |      103 | 5.81e-02 | 1.00e+00 ||     3.23 |     2.77 |   633.48 ||     7.61 | 00h00'00  
        1.75 |  2.50 |      106 |     9028 |      103 | 4.85e-02 | 9.72e-01 ||     3.06 |     2.63 |   668.27 ||     6.98 | 00h00'00  
        2.00 |  2.75 |      125 |     6568 |      103 | 2.99e-02 | 8.24e-01 ||     3.36 |     2.88 |   609.05 ||     7.83 | 00h00'00  
        2.25 |  3.00 |      200 |     4557 |      103 | 1.30e-02 | 5.15e-01 ||     4.30 |     3.68 |   476.63 ||    10.25 | 00h00'00  
        2.50 |  3.25 |      565 |     3849 |      100 | 3.88e-03 | 1.77e-01 ||     8.14 |     6.98 |   251.59 ||    16.54 | 00h00'00  
        2.75 |  3.50 |     2594 |     3024 |      101 | 6.64e-04 | 3.89e-02 ||    15.78 |    13.53 |   129.74 ||    24.63 | 00h00'00  
        3.00 |  3.75 |    18102 |     2265 |      100 | 7.13e-05 | 5.52e-03 ||    32.96 |    28.25 |    62.13 ||    31.41 | 00h00'01  
        3.25 |  4.00 |   179246 |     1712 |      100 | 5.44e-06 | 5.58e-04 ||    48.69 |    41.72 |    42.06 ||    32.47 | 00h00'11  
    # End of the simulation.

## More

See the [AFF3CT Wiki](https://github.com/aff3ct/aff3ct/wiki).