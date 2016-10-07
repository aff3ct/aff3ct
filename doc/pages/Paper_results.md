# Reproduce scientific results
**AFF3CT** is a tool dedicated to the research in the digital communication area.
It helps to materialize some research works. This page lists the different research papers based on **AFF3CT** and gives the guidelines to reproduce the papers results.

## [An efficient, portable and generic library for Successive Cancellation decoding of Polar Codes](https://hal.inria.fr/hal-01203105/document)
*Adrien Cassagne, Bertrand Le Gal, Camille Leroux, Olivier Aumage and Denis Barthou*

**The 28th International Workshop on Languages and Compilers for Parallel Computing (LCPC 2015), September 2015.**

The results presented in this paper required to enable some features directly in the source code of **AFF3CT**.
Open the `src/Tools/Factory/Polar/Factory_decoder_polar_gen.cpp` file and uncomment the `#define` like this:

	/* // GENERATED DECODERS // */
	
	#define ENABLE_SHORT_GENERATED_DECODERS
	
	// RATE 1/2
	#define ENABLE_DECODER_SC_FAST_N4_K2_SNR25
	// #define ENABLE_DECODER_SC_FAST_N8_K4_SNR25
	// #define ENABLE_DECODER_SC_FAST_N16_K8_SNR25
	// #define ENABLE_DECODER_SC_FAST_N32_K16_SNR25
	#define ENABLE_DECODER_SC_FAST_N64_K32_SNR25
	#define ENABLE_DECODER_SC_FAST_N128_K64_SNR25
	#define ENABLE_DECODER_SC_FAST_N256_K128_SNR25
	#define ENABLE_DECODER_SC_FAST_N512_K256_SNR25
	#define ENABLE_DECODER_SC_FAST_N1024_K512_SNR25
	#define ENABLE_DECODER_SC_FAST_N2048_K1024_SNR25
	#define ENABLE_DECODER_SC_FAST_N4096_K2048_SNR25
	#define ENABLE_DECODER_SC_FAST_N4096_K2048_SNR33
	#define ENABLE_DECODER_SC_FAST_N8192_K4096_SNR25
	#define ENABLE_DECODER_SC_FAST_N16384_K8192_SNR25
	#define ENABLE_DECODER_SC_FAST_N32768_K16384_SNR18
	#define ENABLE_DECODER_SC_FAST_N32768_K16384_SNR25
	#define ENABLE_DECODER_SC_FAST_N65536_K32768_SNR25
	// #define ENABLE_DECODER_SC_FAST_N131072_K65536_SNR25
	// #define ENABLE_DECODER_SC_FAST_N262144_K131072_SNR25
	// #define ENABLE_DECODER_SC_FAST_N524288_K262144_SNR25
	// #define ENABLE_DECODER_SC_FAST_N1048576_K524288_SNR25
	
	// RATE 5/6
	#define ENABLE_DECODER_SC_FAST_N4_K3_SNR40
	// #define ENABLE_DECODER_SC_FAST_N8_K7_SNR40
	// #define ENABLE_DECODER_SC_FAST_N16_K13_SNR40
	// #define ENABLE_DECODER_SC_FAST_N32_K27_SNR40
	#define ENABLE_DECODER_SC_FAST_N64_K53_SNR40
	#define ENABLE_DECODER_SC_FAST_N128_K107_SNR40
	#define ENABLE_DECODER_SC_FAST_N256_K213_SNR40
	#define ENABLE_DECODER_SC_FAST_N512_K427_SNR40
	#define ENABLE_DECODER_SC_FAST_N1024_K853_SNR40
	#define ENABLE_DECODER_SC_FAST_N2048_K1707_SNR40
	#define ENABLE_DECODER_SC_FAST_N4096_K3413_SNR40
	#define ENABLE_DECODER_SC_FAST_N8192_K6827_SNR40
	#define ENABLE_DECODER_SC_FAST_N16384_K13653_SNR40
	#define ENABLE_DECODER_SC_FAST_N32768_K27307_SNR40
	#define ENABLE_DECODER_SC_FAST_N65536_K54613_SNR40
	// #define ENABLE_DECODER_SC_FAST_N131072_K109227_SNR40
	// #define ENABLE_DECODER_SC_FAST_N262144_K218453_SNR40
	// #define ENABLE_DECODER_SC_FAST_N524288_K436907_SNR40
	// #define ENABLE_DECODER_SC_FAST_N1048576_K873813_SNR40
	
	// RATE 0.84
	#define ENABLE_DECODER_SC_FAST_N32768_K27568_SNR40
	
	// RATE 9/10
	// #define ENABLE_DECODER_SC_FAST_N2048_K1843_SNR40
	#define ENABLE_DECODER_SC_FAST_N16384_K14746_SNR40
	#define ENABLE_DECODER_SC_FAST_N32768_K29492_SNR40
	...

Now you can recompile **AFF3CT** in release mode (`-Ofast -funroll-loops`) and with the *SSE4.1* or *NEON* instructions. Do not forget to enable the `MULTI_PREC` macro.
Notice that the compilation can take a very long time (~30 min)... 

### Figure 6

Go into the `build` folder.

**Code rate R = 1/2**:

	$ ./bin/aff3ct -p 32 --sim-type BFER --cde-type POLAR -m 2.5 -M 2.5 -K    32 -N    64 --dec-type SC --dec-implem N64_K32_SNR25       --dec-simd INTRA --sim-benchs 2000000
	$ ./bin/aff3ct -p 32 --sim-type BFER --cde-type POLAR -m 2.5 -M 2.5 -K    64 -N   128 --dec-type SC --dec-implem N128_K64_SNR25      --dec-simd INTRA --sim-benchs 2000000
	$ ...
	$ ./bin/aff3ct -p 32 --sim-type BFER --cde-type POLAR -m 2.5 -M 2.5 -K 32768 -N 65536 --dec-type SC --dec-implem N65536_K32768_SNR25 --dec-simd INTRA --sim-benchs 2000

**Code rate R = 5/6**:

	$ ./bin/aff3ct -p 32 --sim-type BFER --cde-type POLAR -m 4.0 -M 4.0 -K    53 -N    64 --dec-type SC --dec-implem N64_K53_SNR40       --dec-simd INTRA --sim-benchs 2000000
	$ ./bin/aff3ct -p 32 --sim-type BFER --cde-type POLAR -m 4.0 -M 4.0 -K   107 -N   128 --dec-type SC --dec-implem N107_K128_SNR40     --dec-simd INTRA --sim-benchs 2000000
	$ ...
	$ ./bin/aff3ct -p 32 --sim-type BFER --cde-type POLAR -m 4.0 -M 4.0 -K 54613 -N 65536 --dec-type SC --dec-implem N65536_K54613_SNR40 --dec-simd INTRA --sim-benchs 2000
	
### Table 2

Go into the `build` folder.

	$ ./bin/aff3ct -p 32 --sim-type BFER --cde-type POLAR -m 4.0 -M 4.0 -K 14746 -N 16384 --dec-type SC --dec-implem N16384_K14746_SNR40 --dec-simd INTRA --sim-benchs 8000
	$ ./bin/aff3ct -p 32 --sim-type BFER --cde-type POLAR -m 4.0 -M 4.0 -K 27568 -N 32768 --dec-type SC --dec-implem N32768_K27568_SNR40 --dec-simd INTRA --sim-benchs 8000
	$ ./bin/aff3ct -p 32 --sim-type BFER --cde-type POLAR -m 4.0 -M 4.0 -K 29492 -N 32768 --dec-type SC --dec-implem N32768_K29492_SNR40 --dec-simd INTRA --sim-benchs 8000
	
### Figure 7

Go into the `build` folder.

**Code rate R = 1/2**:

	$ ./bin/aff3ct -p 8 --sim-type BFER --cde-type POLAR -m 2.5 -M 2.5 -K   128 -N   256 --dec-type SC --dec-implem N256_K128_SNR25     --dec-simd INTER --sim-benchs 200000
	$ ./bin/aff3ct -p 8 --sim-type BFER --cde-type POLAR -m 2.5 -M 2.5 -K   256 -N   512 --dec-type SC --dec-implem N512_K256_SNR25     --dec-simd INTER --sim-benchs 200000
	$ ...
	$ ./bin/aff3ct -p 8 --sim-type BFER --cde-type POLAR -m 2.5 -M 2.5 -K 32768 -N 65536 --dec-type SC --dec-implem N65536_K32768_SNR25 --dec-simd INTER --sim-benchs 800
	
**Code rate R = 5/6**:

	$ ./bin/aff3ct -p 8 --sim-type BFER --cde-type POLAR -m 4.0 -M 4.0 -K   213 -N   256 --dec-type SC --dec-implem N256_K213_SNR40     --dec-simd INTER --sim-benchs 200000
	$ ./bin/aff3ct -p 8 --sim-type BFER --cde-type POLAR -m 4.0 -M 4.0 -K   427 -N   512 --dec-type SC --dec-implem N512_K427_SNR40     --dec-simd INTER --sim-benchs 200000
	$ ...
	$ ./bin/aff3ct -p 8 --sim-type BFER --cde-type POLAR -m 4.0 -M 4.0 -K 54613 -N 65536 --dec-type SC --dec-implem N65536_K54613_SNR40 --dec-simd INTER --sim-benchs 800

