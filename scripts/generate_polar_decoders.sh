#/bin/bash

echo "Generate decoders (rate 1/2)..."
../build/bin/aff3ct --code-type POLAR --simu-type GEN -N       4 -K      2 --snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N4_K2_SNR25.report 2>&1
../build/bin/aff3ct --code-type POLAR --simu-type GEN -N       8 -K      4 --snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N8_K4_SNR25.report 2>&1
../build/bin/aff3ct --code-type POLAR --simu-type GEN -N      16 -K      8 --snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N16_K8_SNR25.report 2>&1
../build/bin/aff3ct --code-type POLAR --simu-type GEN -N      32 -K     16 --snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32_K16_SNR25.report 2>&1
../build/bin/aff3ct --code-type POLAR --simu-type GEN -N      64 -K     32 --snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N64_K32_SNR25.report 2>&1
../build/bin/aff3ct --code-type POLAR --simu-type GEN -N     128 -K     64 --snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N128_K64_SNR25.report 2>&1
../build/bin/aff3ct --code-type POLAR --simu-type GEN -N     256 -K    128 --snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N256_K128_SNR25.report 2>&1
../build/bin/aff3ct --code-type POLAR --simu-type GEN -N     512 -K    256 --snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N512_K256_SNR25.report 2>&1
../build/bin/aff3ct --code-type POLAR --simu-type GEN -N    1024 -K    512 --snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N1024_K512_SNR25.report 2>&1
../build/bin/aff3ct --code-type POLAR --simu-type GEN -N    2048 -K   1024 --snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K1024_SNR25.report 2>&1
../build/bin/aff3ct --code-type POLAR --simu-type GEN -N    4096 -K   2048 --snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N4096_K2048_SNR25.report 2>&1
../build/bin/aff3ct --code-type POLAR --simu-type GEN -N    8192 -K   4096 --snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N8192_K4096_SNR25.report 2>&1
../build/bin/aff3ct --code-type POLAR --simu-type GEN -N   16384 -K   8192 --snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N16384_K8192_SNR25.report 2>&1
../build/bin/aff3ct --code-type POLAR --simu-type GEN -N   32768 -K  16384 --snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K16384_SNR25.report 2>&1
../build/bin/aff3ct --code-type POLAR --simu-type GEN -N   65536 -K  32768 --snr 1.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N65536_K32768_SNR15.report 2>&1
../build/bin/aff3ct --code-type POLAR --simu-type GEN -N   65536 -K  32768 --snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N65536_K32768_SNR25.report 2>&1
../build/bin/aff3ct --code-type POLAR --simu-type GEN -N  131072 -K  65536 --snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N131072_K65536_SNR25.report 2>&1
../build/bin/aff3ct --code-type POLAR --simu-type GEN -N  262144 -K 131072 --snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N262144_K131072_SNR25.report 2>&1
../build/bin/aff3ct --code-type POLAR --simu-type GEN -N  524288 -K 262144 --snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N524288_K262144_SNR25.report 2>&1
../build/bin/aff3ct --code-type POLAR --simu-type GEN -N 1048576 -K 524288 --snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N1048576_K524288_SNR25.report 2>&1

echo "Generate decoders (rate 5/6)..."
../build/bin/aff3ct --code-type POLAR --simu-type GEN -N       4 -K      3 --snr 4.0 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N4_K3_SNR40.report 2>&1
../build/bin/aff3ct --code-type POLAR --simu-type GEN -N       8 -K      7 --snr 4.0 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N8_K7_SNR40.report 2>&1
../build/bin/aff3ct --code-type POLAR --simu-type GEN -N      16 -K     13 --snr 4.0 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N16_K13_SNR40.report 2>&1
../build/bin/aff3ct --code-type POLAR --simu-type GEN -N      32 -K     27 --snr 4.0 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32_K27_SNR40.report 2>&1
../build/bin/aff3ct --code-type POLAR --simu-type GEN -N      64 -K     53 --snr 4.0 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N64_K53_SNR40.report 2>&1
../build/bin/aff3ct --code-type POLAR --simu-type GEN -N     128 -K    107 --snr 4.0 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N128_K107_SNR40.report 2>&1
../build/bin/aff3ct --code-type POLAR --simu-type GEN -N     256 -K    213 --snr 4.0 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N256_K213_SNR40.report 2>&1
../build/bin/aff3ct --code-type POLAR --simu-type GEN -N     512 -K    427 --snr 4.0 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N512_K427_SNR40.report 2>&1
../build/bin/aff3ct --code-type POLAR --simu-type GEN -N    1024 -K    853 --snr 4.0 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N1024_K853_SNR40.report 2>&1
../build/bin/aff3ct --code-type POLAR --simu-type GEN -N    2048 -K   1707 --snr 4.0 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K1707_SNR40.report 2>&1
../build/bin/aff3ct --code-type POLAR --simu-type GEN -N    4096 -K   3413 --snr 4.0 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N4096_K3413_SNR40.report 2>&1
../build/bin/aff3ct --code-type POLAR --simu-type GEN -N    8192 -K   6827 --snr 4.0 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N8192_K6827_SNR40.report 2>&1
../build/bin/aff3ct --code-type POLAR --simu-type GEN -N   16384 -K  13653 --snr 4.0 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N16384_K13653_SNR40.report 2>&1
../build/bin/aff3ct --code-type POLAR --simu-type GEN -N   32768 -K  27307 --snr 4.0 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K27307_SNR40.report 2>&1
../build/bin/aff3ct --code-type POLAR --simu-type GEN -N   65536 -K  54613 --snr 4.0 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N65536_K54613_SNR40.report 2>&1
../build/bin/aff3ct --code-type POLAR --simu-type GEN -N  131072 -K 109227 --snr 4.0 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N131072_K109227_SNR40.report 2>&1
../build/bin/aff3ct --code-type POLAR --simu-type GEN -N  262144 -K 218453 --snr 4.0 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N262144_K218453_SNR40.report 2>&1
../build/bin/aff3ct --code-type POLAR --simu-type GEN -N  524288 -K 436907 --snr 4.0 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N524288_K436907_SNR40.report 2>&1
../build/bin/aff3ct --code-type POLAR --simu-type GEN -N 1048576 -K 873813 --snr 4.0 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N1048576_K873813_SNR40.report 2>&1

echo "Generate decoders N = 2048 (rate 1/10, 2/10, ..., 9/10)..."
../build/bin/aff3ct --code-type POLAR --simu-type GEN -N 2048 -K  205 --snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K205_SNR25.report 2>&1
../build/bin/aff3ct --code-type POLAR --simu-type GEN -N 2048 -K  410 --snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K410_SNR25.report 2>&1
../build/bin/aff3ct --code-type POLAR --simu-type GEN -N 2048 -K  614 --snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K614_SNR25.report 2>&1
../build/bin/aff3ct --code-type POLAR --simu-type GEN -N 2048 -K  819 --snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K819_SNR25.report 2>&1
../build/bin/aff3ct --code-type POLAR --simu-type GEN -N 2048 -K 1024 --snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K1024_SNR25.report 2>&1
../build/bin/aff3ct --code-type POLAR --simu-type GEN -N 2048 -K 1229 --snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K1229_SNR25.report 2>&1
../build/bin/aff3ct --code-type POLAR --simu-type GEN -N 2048 -K 1434 --snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K1434_SNR25.report 2>&1
../build/bin/aff3ct --code-type POLAR --simu-type GEN -N 2048 -K 1638 --snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K1638_SNR25.report 2>&1
../build/bin/aff3ct --code-type POLAR --simu-type GEN -N 2048 -K 1843 --snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K1843_SNR25.report 2>&1

echo "Generate decoders N = 32768 (rate 1/10, 2/10, ..., 9/10)..."
../build/bin/aff3ct --code-type POLAR --simu-type GEN -N 32768 -K  3277 --snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K3277_SNR25.report 2>&1
../build/bin/aff3ct --code-type POLAR --simu-type GEN -N 32768 -K  6554 --snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K6554_SNR25.report 2>&1
../build/bin/aff3ct --code-type POLAR --simu-type GEN -N 32768 -K  9830 --snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K9830_SNR25.report 2>&1
../build/bin/aff3ct --code-type POLAR --simu-type GEN -N 32768 -K 13107 --snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K13107_SNR25.report 2>&1
../build/bin/aff3ct --code-type POLAR --simu-type GEN -N 32768 -K 16384 --snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K16384_SNR25.report 2>&1
../build/bin/aff3ct --code-type POLAR --simu-type GEN -N 32768 -K 19661 --snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K19661_SNR25.report 2>&1
../build/bin/aff3ct --code-type POLAR --simu-type GEN -N 32768 -K 22938 --snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K22938_SNR25.report 2>&1
../build/bin/aff3ct --code-type POLAR --simu-type GEN -N 32768 -K 26214 --snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K26214_SNR25.report 2>&1
../build/bin/aff3ct --code-type POLAR --simu-type GEN -N 32768 -K 29491 --snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K29491_SNR25.report 2>&1

echo "Generate decoders (other rates)..."
../build/bin/aff3ct --code-type POLAR --simu-type GEN -N  2048 -K   410 --snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K410_SNR25.report 2>&1
../build/bin/aff3ct --code-type POLAR --simu-type GEN -N  2048 -K  1843 --snr 4.0 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K1843_SNR40.report 2>&1
../build/bin/aff3ct --code-type POLAR --simu-type GEN -N 16384 -K 14746 --snr 4.0 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N16384_K14746_SNR40.report 2>&1
../build/bin/aff3ct --code-type POLAR --simu-type GEN -N 32768 -K 27568 --snr 4.0 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K27568_SNR40.report 2>&1
../build/bin/aff3ct --code-type POLAR --simu-type GEN -N 32768 -K 29492 --snr 4.0 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K29492_SNR40.report 2>&1

echo "Generate graphs (from *.dot to *.dot.pdf, rate 1/2)..."
dot -Tpdf ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N4_K2_SNR25.dot -O
dot -Tpdf ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N4_K2_SNR25.dot -O
dot -Tpdf ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N8_K4_SNR25.dot -O
dot -Tpdf ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N16_K8_SNR25.dot -O
dot -Tpdf ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32_K16_SNR25.dot -O
dot -Tpdf ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N64_K32_SNR25.dot -O
dot -Tpdf ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N128_K64_SNR25.dot -O
dot -Tpdf ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N256_K128_SNR25.dot -O
dot -Tpdf ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N512_K256_SNR25.dot -O
dot -Tpdf ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N1024_K512_SNR25.dot -O
dot -Tpdf ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K1024_SNR25.dot -O
dot -Tpdf ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N4096_K2048_SNR25.dot -O

echo "Generate graphs (from *.dot to *.dot.pdf, rate 5/6)..."
dot -Tpdf ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N4_K3_SNR40.dot -O
dot -Tpdf ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N8_K7_SNR40.dot -O
dot -Tpdf ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N16_K13_SNR40.dot -O
dot -Tpdf ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32_K27_SNR40.dot -O
dot -Tpdf ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N64_K53_SNR40.dot -O
dot -Tpdf ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N128_K107_SNR40.dot -O
dot -Tpdf ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N256_K213_SNR40.dot -O
dot -Tpdf ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N512_K427_SNR40.dot -O
dot -Tpdf ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N1024_K853_SNR40.dot -O
dot -Tpdf ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K1707_SNR40.dot -O
dot -Tpdf ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N4096_K3413_SNR40.dot -O

echo "Generate short graphs (from *.short.dot to *.short.dot.pdf, rate 1/2)..."
dot -Tpdf ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N4_K2_SNR25.short.dot -O
dot -Tpdf ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N4_K2_SNR25.short.dot -O
dot -Tpdf ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N8_K4_SNR25.short.dot -O
dot -Tpdf ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N16_K8_SNR25.short.dot -O
dot -Tpdf ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32_K16_SNR25.short.dot -O
dot -Tpdf ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N64_K32_SNR25.short.dot -O
dot -Tpdf ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N128_K64_SNR25.short.dot -O
dot -Tpdf ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N256_K128_SNR25.short.dot -O
dot -Tpdf ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N512_K256_SNR25.short.dot -O
dot -Tpdf ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N1024_K512_SNR25.short.dot -O
dot -Tpdf ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K1024_SNR25.short.dot -O
dot -Tpdf ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N4096_K2048_SNR25.short.dot -O
dot -Tpdf ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N8192_K4096_SNR25.short.dot -O
dot -Tpdf ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N16384_K8192_SNR25.short.dot -O
dot -Tpdf ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K16384_SNR25.short.dot -O
dot -Tpdf ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N65536_K32768_SNR15.short.dot -O
dot -Tpdf ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N65536_K32768_SNR25.short.dot -O

echo "Generate short graphs (from *.short.dot to *.short.dot.pdf, rate 5/6)..."
dot -Tpdf ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N4_K3_SNR40.short.dot -O
dot -Tpdf ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N8_K7_SNR40.short.dot -O
dot -Tpdf ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N16_K13_SNR40.short.dot -O
dot -Tpdf ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32_K27_SNR40.short.dot -O
dot -Tpdf ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N64_K53_SNR40.short.dot -O
dot -Tpdf ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N128_K107_SNR40.short.dot -O
dot -Tpdf ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N256_K213_SNR40.short.dot -O
dot -Tpdf ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N512_K427_SNR40.short.dot -O
dot -Tpdf ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N1024_K853_SNR40.short.dot -O
dot -Tpdf ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K1707_SNR40.short.dot -O
dot -Tpdf ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N4096_K3413_SNR40.short.dot -O
dot -Tpdf ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N8192_K6827_SNR40.short.dot -O
dot -Tpdf ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N16384_K13653_SNR40.short.dot -O
dot -Tpdf ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K27307_SNR40.short.dot -O
dot -Tpdf ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N65536_K54613_SNR40.short.dot -O
