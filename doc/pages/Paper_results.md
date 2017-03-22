# Reproduce scientific results
**AFF3CT** is a tool dedicated to the research in the digital communication area.
It helps to materialize some research works. This page lists the different research papers based on **AFF3CT** and gives the guidelines to reproduce the papers results.

## [Beyond Gbps Turbo decoder on multi-core CPUs ](https://hal.inria.fr/hal-01363980/document)
*Adrien Cassagne, Thibaud Tonnellier, Camille Leroux, Bertrand Le Gal, Olivier Aumage and Denis Barthou*

**The 10th International Symposium on Turbo Codes and Iterative Information Processing (ISTC 2016), September 2016.**

Compile **AFF3CT** in release mode (`-Ofast -funroll-loops`) and with the *SSE4.1* or *AVX2* instructions. Do not forget to enable the `-DMULTI_PREC` macro.

### Figure 2

Go into the `build` folder.

	$ ./bin/aff3ct -p 32 --sim-type BFER --cde-type TURBO -m 0.0 -M 2.01 -K 6144 -N 18432 --src-type RAND_FAST --mod-type BPSK_FAST --chn-type AWGN_FAST                                         --dec-type LTE --dec-implem VERY_FAST --dec-simd INTER -i 6 --itl-type LTE --dec-sf LTE_VEC --dec-max MAX
	$ ./bin/aff3ct -p 16 --sim-type BFER --cde-type TURBO -m 0.0 -M 2.01 -K 6144 -N 18432 --src-type RAND_FAST --mod-type BPSK_FAST --chn-type AWGN_FAST --dmod-no-sig2 --qnt-bits 6 --qnt-dec 3 --dec-type LTE --dec-implem VERY_FAST --dec-simd INTER -i 6 --itl-type LTE --dec-sf LTE_VEC --dec-max MAX
	$ ./bin/aff3ct -p  8 --sim-type BFER --cde-type TURBO -m 0.0 -M 2.01 -K 6144 -N 18432 --src-type RAND_FAST --mod-type BPSK_FAST --chn-type AWGN_FAST --dmod-no-sig2 --qnt-bits 6 --qnt-dec 2 --dec-type LTE --dec-implem VERY_FAST --dec-simd INTER -i 6 --itl-type LTE --dec-sf LTE_VEC --dec-max MAX

### Figure 3

Notice that for this figure **AFF3CT** has been compiled twice: 1) with the SSE4.1 instructions, 2) with the AVX2 instructions.
The number of threads can vary with the `-t` argument from the command line.

Go into the `build` folder.

	$ ./bin/aff3ct -p 8 --sim-type BFER --cde-type TURBO -m 1.0 -M 1.0 -K 1024 -N  3072 --dec-type LTE --dec-implem VERY_FAST --dec-simd INTER -i 6 --itl-type LTE --dec-sf LTE_VEC --dec-max MAX --sim-benchs 1000
	$ ./bin/aff3ct -p 8 --sim-type BFER --cde-type TURBO -m 1.0 -M 1.0 -K 2048 -N  6144 --dec-type LTE --dec-implem VERY_FAST --dec-simd INTER -i 6 --itl-type LTE --dec-sf LTE_VEC --dec-max MAX --sim-benchs 1000
	$ ...
	$ ./bin/aff3ct -p 8 --sim-type BFER --cde-type TURBO -m 1.0 -M 1.0 -K 6144 -N 18432 --dec-type LTE --dec-implem VERY_FAST --dec-simd INTER -i 6 --itl-type LTE --dec-sf LTE_VEC --dec-max MAX --sim-benchs  500

### Table 2

Notice that for this figure **AFF3CT** has been compiled with the SSE4.1 instructions. The number of threads can vary with the `-t` argument from the command line.

Go into the `build` folder.

	$ ./bin/aff3ct -p 16 --sim-type BFER --cde-type TURBO -m 0.7 -M 0.7 -K 6144 -N 18432 --dec-type LTE --dec-implem VERY_FAST --dec-simd INTER -i 6 --itl-type LTE --dec-sf LTE_VEC --dec-max MAX --sim-benchs 100
	$ ./bin/aff3ct -p  8 --sim-type BFER --cde-type TURBO -m 0.7 -M 0.7 -K 6144 -N 18432 --dec-type LTE --dec-implem VERY_FAST --dec-simd INTER -i 6 --itl-type LTE --dec-sf LTE_VEC --dec-max MAX --sim-benchs 200

-----------------------------------------------------------------------------------------------------------------------

## [Energy consumption analysis of software Polar Decoders on low power processors ](http://www.eurasip.org/Proceedings/Eusipco/Eusipco2016/papers/1570252182.pdf)
*Adrien Cassagne, Olivier Aumage, Camille Leroux, Denis Barthou and Bertrand Le Gal*

**The 24nd European Signal Processing Conference (EUSIPCO 2016), September 2016.**

The results presented in this paper require to enable some features directly in the source code of **AFF3CT**.
Open the `src/Tools/Factory/Polar/Factory_decoder_polar_gen.cpp` file and uncomment the `#define` like this:

	/* // GENERATED DECODERS // */
	
	#define ENABLE_SHORT_GENERATED_DECODERS
	
	// RATE 1/2
	#define ENABLE_DECODER_SC_FAST_N4_K2_SNR25
	// #define ENABLE_DECODER_SC_FAST_N8_K4_SNR25
	// #define ENABLE_DECODER_SC_FAST_N16_K8_SNR25
	// #define ENABLE_DECODER_SC_FAST_N32_K16_SNR25
	// #define ENABLE_DECODER_SC_FAST_N64_K32_SNR25
	// #define ENABLE_DECODER_SC_FAST_N128_K64_SNR25
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
	#define ENABLE_DECODER_SC_FAST_N131072_K65536_SNR25
	#define ENABLE_DECODER_SC_FAST_N262144_K131072_SNR25
	#define ENABLE_DECODER_SC_FAST_N524288_K262144_SNR25
	#define ENABLE_DECODER_SC_FAST_N1048576_K524288_SNR25
	
	// RATE 5/6
	#define ENABLE_DECODER_SC_FAST_N4_K3_SNR40
	// #define ENABLE_DECODER_SC_FAST_N8_K7_SNR40
	// #define ENABLE_DECODER_SC_FAST_N16_K13_SNR40
	// #define ENABLE_DECODER_SC_FAST_N32_K27_SNR40
	// #define ENABLE_DECODER_SC_FAST_N64_K53_SNR40
	// #define ENABLE_DECODER_SC_FAST_N128_K107_SNR40
	// #define ENABLE_DECODER_SC_FAST_N256_K213_SNR40
	// #define ENABLE_DECODER_SC_FAST_N512_K427_SNR40
	// #define ENABLE_DECODER_SC_FAST_N1024_K853_SNR40
	// #define ENABLE_DECODER_SC_FAST_N2048_K1707_SNR40
	// #define ENABLE_DECODER_SC_FAST_N4096_K3413_SNR40
	// #define ENABLE_DECODER_SC_FAST_N8192_K6827_SNR40
	// #define ENABLE_DECODER_SC_FAST_N16384_K13653_SNR40
	#define ENABLE_DECODER_SC_FAST_N32768_K27307_SNR40
	// #define ENABLE_DECODER_SC_FAST_N65536_K54613_SNR40
	// #define ENABLE_DECODER_SC_FAST_N131072_K109227_SNR40
	// #define ENABLE_DECODER_SC_FAST_N262144_K218453_SNR40
	// #define ENABLE_DECODER_SC_FAST_N524288_K436907_SNR40
	// #define ENABLE_DECODER_SC_FAST_N1048576_K873813_SNR40
	...

The dynamic SC decoder requires an accurate frozen bits generation method which is not included in **AFF3CT** for now.
In this paper we used an external solution to construct the frozen bits following the Tal & Vardy method ([How to Construct Polar Codes](https://arxiv.org/pdf/1105.6164.pdf)).
To reproduce the results you have to unzip the `conf/cde/awgn_polar_codes.zip` in the same folder (`conf/cde/`).

Now you can recompile **AFF3CT** in release mode (`-Ofast -funroll-loops`) and with the *SSE4.1* or *NEON* instructions. Do not forget to enable the `-DPREC_8_BIT` macro.
Notice that the compilation can take a very long time (~30 min)... 

### Figure 2

Go into the `build` folder.

**BER/FER for the dynamic version of the SC decoder**:

	$ ./bin/aff3ct -p 8 --sim-type BFER --cde-type POLAR -m 1.0 -M 3.21 -s 0.2 -N  4096 -K  2048 --cde-fb-gen-method TV --src-type AZCW --chn-type AWGN_FAST --dec-algo SC --dec-implem FAST --dec-simd INTER
	$ ./bin/aff3ct -p 8 --sim-type BFER --cde-type POLAR -m 2.6 -M 4.01 -s 0.2 -N 32768 -K 27307 --cde-fb-gen-method TV --src-type AZCW --chn-type AWGN_FAST --dec-algo SC --dec-implem FAST --dec-simd INTER
	
**BER/FER for the generated version of the SC decoder**:

	$ ./bin/aff3ct -p 8 --sim-type BFER --cde-type POLAR -m 1.0 -M 3.21 -s 0.2 -N  4096 -K  2048 --src-type AZCW --chn-type AWGN_FAST --dec-algo SC --dec-implem N4096_K2048_SNR33   --dec-simd INTER
	$ ./bin/aff3ct -p 8 --sim-type BFER --cde-type POLAR -m 2.6 -M 4.01 -s 0.2 -N 32768 -K 27307 --src-type AZCW --chn-type AWGN_FAST --dec-algo SC --dec-implem N32768_K27307_SNR40 --dec-simd INTER
	
### Table 2

Go into the `build` folder.

**Dynamic and sequential version of the SC decoder**:

	$ ./bin/aff3ct -p 8 --sim-type BFER --cde-type POLAR -m 3.20 -M 3.20 -N  4096 -K  2048 --cde-fb-gen-method TV --dec-algo SC --dec-implem FAST --sim-benchs 40000 -t 1

**Dynamic and intra-frame SIMD version of the SC decoder**:

	$ ./bin/aff3ct -p 8 --sim-type BFER --cde-type POLAR -m 3.20 -M 3.20 -N  4096 -K  2048 --cde-fb-gen-method TV --dec-algo SC --dec-implem FAST --dec-simd INTRA --sim-benchs 40000 -t 1

**Dynamic and inter-frame SIMD version of the SC decoder**:

	$ ./bin/aff3ct -p 8 --sim-type BFER --cde-type POLAR -m 3.20 -M 3.20 -N  4096 -K  2048 --cde-fb-gen-method TV --dec-algo SC --dec-implem FAST --dec-simd INTER --sim-benchs 40000 -t 1

### Table 3

Go into the `build` folder.

	$ ./bin/aff3ct -p 8 --sim-type BFER --cde-type POLAR -m 4.00 -M 4.00 -N 32768 -K 27307 --cde-fb-gen-method TV --dec-algo SC --dec-implem FAST --dec-simd INTRA --sim-benchs 20000 -t 1

### Figure 3

Go into the `build` folder.

**Dynamic and intra-frame SIMD**:

	$ ./bin/aff3ct -p 8 --sim-type BFER --cde-type POLAR -m 2.50 -M 2.50 -N     256 -K    128 --cde-fb-gen-method TV --dec-algo SC --dec-implem FAST --dec-simd INTRA --sim-benchs 800000 -t 1
	$ ./bin/aff3ct -p 8 --sim-type BFER --cde-type POLAR -m 2.50 -M 2.50 -N     512 -K    256 --cde-fb-gen-method TV --dec-algo SC --dec-implem FAST --dec-simd INTRA --sim-benchs 800000 -t 1
	$ ...
	$ ./bin/aff3ct -p 8 --sim-type BFER --cde-type POLAR -m 2.50 -M 2.50 -N 1048576 -K 524288 --cde-fb-gen-method TV --dec-algo SC --dec-implem FAST --dec-simd INTRA --sim-benchs 1000   -t 1

**Dynamic and inter-frame SIMD**:

	$ ./bin/aff3ct -p 8 --sim-type BFER --cde-type POLAR -m 2.50 -M 2.50 -N     256 -K    128 --cde-fb-gen-method TV --dec-algo SC --dec-implem FAST --dec-simd INTER --sim-benchs 800000 -t 1
	$ ./bin/aff3ct -p 8 --sim-type BFER --cde-type POLAR -m 2.50 -M 2.50 -N     512 -K    256 --cde-fb-gen-method TV --dec-algo SC --dec-implem FAST --dec-simd INTER --sim-benchs 800000 -t 1
	$ ...
	$ ./bin/aff3ct -p 8 --sim-type BFER --cde-type POLAR -m 2.50 -M 2.50 -N 1048576 -K 524288 --cde-fb-gen-method TV --dec-algo SC --dec-implem FAST --dec-simd INTER --sim-benchs 50     -t 1

**Generated and intra-frame SIMD**:

	$ ./bin/aff3ct -p 8 --sim-type BFER --cde-type POLAR -m 2.50 -M 2.50 -N     256 -K    128 --dec-algo SC --dec-implem N256_K128_SNR25        --dec-simd INTRA --sim-benchs 800000 -t 1
	$ ./bin/aff3ct -p 8 --sim-type BFER --cde-type POLAR -m 2.50 -M 2.50 -N     512 -K    256 --dec-algo SC --dec-implem N512_K256_SNR25        --dec-simd INTRA --sim-benchs 800000 -t 1
	$ ...
	$ ./bin/aff3ct -p 8 --sim-type BFER --cde-type POLAR -m 2.50 -M 2.50 -N 1048576 -K 524288 --dec-algo SC --dec-implem N1048576_K524288_SNR25 --dec-simd INTRA --sim-benchs 1000   -t 1

**Generated and inter-frame SIMD**:

	$ ./bin/aff3ct -p 8 --sim-type BFER --cde-type POLAR -m 2.50 -M 2.50 -N     256 -K    128 --dec-algo SC --dec-implem N256_K128_SNR25        --dec-simd INTER --sim-benchs 800000 -t 1
	$ ./bin/aff3ct -p 8 --sim-type BFER --cde-type POLAR -m 2.50 -M 2.50 -N     512 -K    256 --dec-algo SC --dec-implem N512_K256_SNR25        --dec-simd INTER --sim-benchs 800000 -t 1
	$ ...
	$ ./bin/aff3ct -p 8 --sim-type BFER --cde-type POLAR -m 2.50 -M 2.50 -N 1048576 -K 524288 --dec-algo SC --dec-implem N1048576_K524288_SNR25 --dec-simd INTER --sim-benchs 50     -t 1

### Figure 4

Go into the `build` folder.

**Dynamic and intra-frame SIMD version of the SC decoder**:

	$ ./bin/aff3ct -p 8 --sim-type BFER --cde-type POLAR -m 3.20 -M 3.20 -N  4096 -K  2048 --cde-fb-gen-method TV --dec-algo SC --dec-implem FAST --dec-simd INTRA --sim-benchs 40000 -t 1

**Dynamic and inter-frame SIMD version of the SC decoder**:

	$ ./bin/aff3ct -p 8 --sim-type BFER --cde-type POLAR -m 3.20 -M 3.20 -N  4096 -K  2048 --cde-fb-gen-method TV --dec-algo SC --dec-implem FAST --dec-simd INTER --sim-benchs 40000 -t 1

### Figure 5

Go into the `build` folder.

**Dynamic and intra-frame SIMD version of the SC decoder**:

	$ ./bin/aff3ct -p 8 --sim-type BFER --cde-type POLAR -m 2.50 -M 2.50 -N 32768 -K  3277 --cde-fb-gen-method TV --dec-algo SC --dec-implem FAST --dec-simd INTRA --sim-benchs 40000 -t 1
	$ ./bin/aff3ct -p 8 --sim-type BFER --cde-type POLAR -m 2.50 -M 2.50 -N 32768 -K  6554 --cde-fb-gen-method TV --dec-algo SC --dec-implem FAST --dec-simd INTRA --sim-benchs 40000 -t 1
	$ ...
	$ ./bin/aff3ct -p 8 --sim-type BFER --cde-type POLAR -m 2.50 -M 2.50 -N 32768 -K 29491 --cde-fb-gen-method TV --dec-algo SC --dec-implem FAST --dec-simd INTRA --sim-benchs 40000 -t 1

**Dynamic and inter-frame SIMD version of the SC decoder**:

	$ ./bin/aff3ct -p 8 --sim-type BFER --cde-type POLAR -m 2.50 -M 2.50 -N 32768 -K  3277 --cde-fb-gen-method TV --dec-algo SC --dec-implem FAST --dec-simd INTER --sim-benchs 6000 -t 1
	$ ./bin/aff3ct -p 8 --sim-type BFER --cde-type POLAR -m 2.50 -M 2.50 -N 32768 -K  6554 --cde-fb-gen-method TV --dec-algo SC --dec-implem FAST --dec-simd INTER --sim-benchs 6000 -t 1
	$ ...
	$ ./bin/aff3ct -p 8 --sim-type BFER --cde-type POLAR -m 2.50 -M 2.50 -N 32768 -K 29491 --cde-fb-gen-method TV --dec-algo SC --dec-implem FAST --dec-simd INTER --sim-benchs 6000 -t 1

-----------------------------------------------------------------------------------------------------------------------

## [An efficient, portable and generic library for Successive Cancellation decoding of Polar Codes](https://hal.inria.fr/hal-01203105/document)
*Adrien Cassagne, Bertrand Le Gal, Camille Leroux, Olivier Aumage and Denis Barthou*

**The 28th International Workshop on Languages and Compilers for Parallel Computing (LCPC 2015), September 2015.**

The results presented in this paper require to enable some features directly in the source code of **AFF3CT**.
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

Now you can recompile **AFF3CT** in release mode (`-Ofast -funroll-loops`) and with the *SSE4.1* or *NEON* instructions. Do not forget to enable the `-DMULTI_PREC` macro.
Notice that the compilation can take a very long time (~30 min)... 

### Figure 6

Go into the `build` folder.

**Code rate R = 1/2**:

	$ ./bin/aff3ct -p 32 --sim-type BFER --cde-type POLAR -m 2.5 -M 2.5 -K    32 -N    64 --dec-type SC --dec-implem N64_K32_SNR25       --dec-simd INTRA --sim-benchs 2000000 -t 1
	$ ./bin/aff3ct -p 32 --sim-type BFER --cde-type POLAR -m 2.5 -M 2.5 -K    64 -N   128 --dec-type SC --dec-implem N128_K64_SNR25      --dec-simd INTRA --sim-benchs 2000000 -t 1
	$ ...
	$ ./bin/aff3ct -p 32 --sim-type BFER --cde-type POLAR -m 2.5 -M 2.5 -K 32768 -N 65536 --dec-type SC --dec-implem N65536_K32768_SNR25 --dec-simd INTRA --sim-benchs 2000    -t 1

**Code rate R = 5/6**:

	$ ./bin/aff3ct -p 32 --sim-type BFER --cde-type POLAR -m 4.0 -M 4.0 -K    53 -N    64 --dec-type SC --dec-implem N64_K53_SNR40       --dec-simd INTRA --sim-benchs 2000000 -t 1
	$ ./bin/aff3ct -p 32 --sim-type BFER --cde-type POLAR -m 4.0 -M 4.0 -K   107 -N   128 --dec-type SC --dec-implem N107_K128_SNR40     --dec-simd INTRA --sim-benchs 2000000 -t 1
	$ ...
	$ ./bin/aff3ct -p 32 --sim-type BFER --cde-type POLAR -m 4.0 -M 4.0 -K 54613 -N 65536 --dec-type SC --dec-implem N65536_K54613_SNR40 --dec-simd INTRA --sim-benchs 2000    -t 1
	
### Table 2

Go into the `build` folder.

	$ ./bin/aff3ct -p 32 --sim-type BFER --cde-type POLAR -m 4.0 -M 4.0 -K 14746 -N 16384 --dec-type SC --dec-implem N16384_K14746_SNR40 --dec-simd INTRA --sim-benchs 8000 -t 1
	$ ./bin/aff3ct -p 32 --sim-type BFER --cde-type POLAR -m 4.0 -M 4.0 -K 27568 -N 32768 --dec-type SC --dec-implem N32768_K27568_SNR40 --dec-simd INTRA --sim-benchs 8000 -t 1
	$ ./bin/aff3ct -p 32 --sim-type BFER --cde-type POLAR -m 4.0 -M 4.0 -K 29492 -N 32768 --dec-type SC --dec-implem N32768_K29492_SNR40 --dec-simd INTRA --sim-benchs 8000 -t 1
	
### Figure 7

Go into the `build` folder.

**Code rate R = 1/2**:

	$ ./bin/aff3ct -p 8 --sim-type BFER --cde-type POLAR -m 2.5 -M 2.5 -K   128 -N   256 --dec-type SC --dec-implem N256_K128_SNR25     --dec-simd INTER --sim-benchs 200000 -t 1
	$ ./bin/aff3ct -p 8 --sim-type BFER --cde-type POLAR -m 2.5 -M 2.5 -K   256 -N   512 --dec-type SC --dec-implem N512_K256_SNR25     --dec-simd INTER --sim-benchs 200000 -t 1
	$ ...
	$ ./bin/aff3ct -p 8 --sim-type BFER --cde-type POLAR -m 2.5 -M 2.5 -K 32768 -N 65536 --dec-type SC --dec-implem N65536_K32768_SNR25 --dec-simd INTER --sim-benchs 800    -t 1
	
**Code rate R = 5/6**:

	$ ./bin/aff3ct -p 8 --sim-type BFER --cde-type POLAR -m 4.0 -M 4.0 -K   213 -N   256 --dec-type SC --dec-implem N256_K213_SNR40     --dec-simd INTER --sim-benchs 200000 -t 1
	$ ./bin/aff3ct -p 8 --sim-type BFER --cde-type POLAR -m 4.0 -M 4.0 -K   427 -N   512 --dec-type SC --dec-implem N512_K427_SNR40     --dec-simd INTER --sim-benchs 200000 -t 1
	$ ...
	$ ./bin/aff3ct -p 8 --sim-type BFER --cde-type POLAR -m 4.0 -M 4.0 -K 54613 -N 65536 --dec-type SC --dec-implem N65536_K54613_SNR40 --dec-simd INTER --sim-benchs 800    -t 1

