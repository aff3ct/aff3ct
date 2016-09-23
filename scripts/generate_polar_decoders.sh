#/bin/bash

echo "Generate decoders (rate 1/2)..."
../build/bin/aff3ct --cde-type POLAR --sim-type GEN -N       4 -K      2 --dec-snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N4_K2_SNR25.report 2>&1
../build/bin/aff3ct --cde-type POLAR --sim-type GEN -N       8 -K      4 --dec-snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N8_K4_SNR25.report 2>&1
../build/bin/aff3ct --cde-type POLAR --sim-type GEN -N      16 -K      8 --dec-snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N16_K8_SNR25.report 2>&1
../build/bin/aff3ct --cde-type POLAR --sim-type GEN -N      32 -K     16 --dec-snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32_K16_SNR25.report 2>&1
../build/bin/aff3ct --cde-type POLAR --sim-type GEN -N      64 -K     32 --dec-snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N64_K32_SNR25.report 2>&1
../build/bin/aff3ct --cde-type POLAR --sim-type GEN -N     128 -K     64 --dec-snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N128_K64_SNR25.report 2>&1
../build/bin/aff3ct --cde-type POLAR --sim-type GEN -N     256 -K    128 --dec-snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N256_K128_SNR25.report 2>&1
../build/bin/aff3ct --cde-type POLAR --sim-type GEN -N     512 -K    256 --dec-snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N512_K256_SNR25.report 2>&1
../build/bin/aff3ct --cde-type POLAR --sim-type GEN -N    1024 -K    512 --dec-snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N1024_K512_SNR25.report 2>&1
../build/bin/aff3ct --cde-type POLAR --sim-type GEN -N    2048 -K   1024 --dec-snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K1024_SNR25.report 2>&1
../build/bin/aff3ct --cde-type POLAR --sim-type GEN -N    4096 -K   2048 --dec-snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N4096_K2048_SNR25.report 2>&1
../build/bin/aff3ct --cde-type POLAR --sim-type GEN -N    8192 -K   4096 --dec-snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N8192_K4096_SNR25.report 2>&1
../build/bin/aff3ct --cde-type POLAR --sim-type GEN -N   16384 -K   8192 --dec-snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N16384_K8192_SNR25.report 2>&1
../build/bin/aff3ct --cde-type POLAR --sim-type GEN -N   32768 -K  16384 --dec-snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K16384_SNR25.report 2>&1
../build/bin/aff3ct --cde-type POLAR --sim-type GEN -N   65536 -K  32768 --dec-snr 1.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N65536_K32768_SNR15.report 2>&1
../build/bin/aff3ct --cde-type POLAR --sim-type GEN -N   65536 -K  32768 --dec-snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N65536_K32768_SNR25.report 2>&1
../build/bin/aff3ct --cde-type POLAR --sim-type GEN -N  131072 -K  65536 --dec-snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N131072_K65536_SNR25.report 2>&1
../build/bin/aff3ct --cde-type POLAR --sim-type GEN -N  262144 -K 131072 --dec-snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N262144_K131072_SNR25.report 2>&1
../build/bin/aff3ct --cde-type POLAR --sim-type GEN -N  524288 -K 262144 --dec-snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N524288_K262144_SNR25.report 2>&1
../build/bin/aff3ct --cde-type POLAR --sim-type GEN -N 1048576 -K 524288 --dec-snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N1048576_K524288_SNR25.report 2>&1

echo "Generate decoders (rate 5/6)..."
../build/bin/aff3ct --cde-type POLAR --sim-type GEN -N       4 -K      3 --dec-snr 4.0 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N4_K3_SNR40.report 2>&1
../build/bin/aff3ct --cde-type POLAR --sim-type GEN -N       8 -K      7 --dec-snr 4.0 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N8_K7_SNR40.report 2>&1
../build/bin/aff3ct --cde-type POLAR --sim-type GEN -N      16 -K     13 --dec-snr 4.0 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N16_K13_SNR40.report 2>&1
../build/bin/aff3ct --cde-type POLAR --sim-type GEN -N      32 -K     27 --dec-snr 4.0 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32_K27_SNR40.report 2>&1
../build/bin/aff3ct --cde-type POLAR --sim-type GEN -N      64 -K     53 --dec-snr 4.0 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N64_K53_SNR40.report 2>&1
../build/bin/aff3ct --cde-type POLAR --sim-type GEN -N     128 -K    107 --dec-snr 4.0 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N128_K107_SNR40.report 2>&1
../build/bin/aff3ct --cde-type POLAR --sim-type GEN -N     256 -K    213 --dec-snr 4.0 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N256_K213_SNR40.report 2>&1
../build/bin/aff3ct --cde-type POLAR --sim-type GEN -N     512 -K    427 --dec-snr 4.0 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N512_K427_SNR40.report 2>&1
../build/bin/aff3ct --cde-type POLAR --sim-type GEN -N    1024 -K    853 --dec-snr 4.0 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N1024_K853_SNR40.report 2>&1
../build/bin/aff3ct --cde-type POLAR --sim-type GEN -N    2048 -K   1707 --dec-snr 4.0 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K1707_SNR40.report 2>&1
../build/bin/aff3ct --cde-type POLAR --sim-type GEN -N    4096 -K   3413 --dec-snr 4.0 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N4096_K3413_SNR40.report 2>&1
../build/bin/aff3ct --cde-type POLAR --sim-type GEN -N    8192 -K   6827 --dec-snr 4.0 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N8192_K6827_SNR40.report 2>&1
../build/bin/aff3ct --cde-type POLAR --sim-type GEN -N   16384 -K  13653 --dec-snr 4.0 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N16384_K13653_SNR40.report 2>&1
../build/bin/aff3ct --cde-type POLAR --sim-type GEN -N   32768 -K  27307 --dec-snr 4.0 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K27307_SNR40.report 2>&1
../build/bin/aff3ct --cde-type POLAR --sim-type GEN -N   65536 -K  54613 --dec-snr 4.0 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N65536_K54613_SNR40.report 2>&1
../build/bin/aff3ct --cde-type POLAR --sim-type GEN -N  131072 -K 109227 --dec-snr 4.0 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N131072_K109227_SNR40.report 2>&1
../build/bin/aff3ct --cde-type POLAR --sim-type GEN -N  262144 -K 218453 --dec-snr 4.0 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N262144_K218453_SNR40.report 2>&1
../build/bin/aff3ct --cde-type POLAR --sim-type GEN -N  524288 -K 436907 --dec-snr 4.0 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N524288_K436907_SNR40.report 2>&1
../build/bin/aff3ct --cde-type POLAR --sim-type GEN -N 1048576 -K 873813 --dec-snr 4.0 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N1048576_K873813_SNR40.report 2>&1

echo "Generate decoders N = 2048 (rate 1/10, 2/10, ..., 9/10)..."
../build/bin/aff3ct --cde-type POLAR --sim-type GEN -N 2048 -K  205 --dec-snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K205_SNR25.report 2>&1
../build/bin/aff3ct --cde-type POLAR --sim-type GEN -N 2048 -K  410 --dec-snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K410_SNR25.report 2>&1
../build/bin/aff3ct --cde-type POLAR --sim-type GEN -N 2048 -K  614 --dec-snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K614_SNR25.report 2>&1
../build/bin/aff3ct --cde-type POLAR --sim-type GEN -N 2048 -K  819 --dec-snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K819_SNR25.report 2>&1
../build/bin/aff3ct --cde-type POLAR --sim-type GEN -N 2048 -K 1024 --dec-snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K1024_SNR25.report 2>&1
../build/bin/aff3ct --cde-type POLAR --sim-type GEN -N 2048 -K 1229 --dec-snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K1229_SNR25.report 2>&1
../build/bin/aff3ct --cde-type POLAR --sim-type GEN -N 2048 -K 1434 --dec-snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K1434_SNR25.report 2>&1
../build/bin/aff3ct --cde-type POLAR --sim-type GEN -N 2048 -K 1638 --dec-snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K1638_SNR25.report 2>&1
../build/bin/aff3ct --cde-type POLAR --sim-type GEN -N 2048 -K 1843 --dec-snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K1843_SNR25.report 2>&1

echo "Generate decoders N = 32768 (rate 1/10, 2/10, ..., 9/10)..."
../build/bin/aff3ct --cde-type POLAR --sim-type GEN -N 32768 -K  3277 --dec-snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K3277_SNR25.report 2>&1
../build/bin/aff3ct --cde-type POLAR --sim-type GEN -N 32768 -K  6554 --dec-snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K6554_SNR25.report 2>&1
../build/bin/aff3ct --cde-type POLAR --sim-type GEN -N 32768 -K  9830 --dec-snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K9830_SNR25.report 2>&1
../build/bin/aff3ct --cde-type POLAR --sim-type GEN -N 32768 -K 13107 --dec-snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K13107_SNR25.report 2>&1
../build/bin/aff3ct --cde-type POLAR --sim-type GEN -N 32768 -K 16384 --dec-snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K16384_SNR25.report 2>&1
../build/bin/aff3ct --cde-type POLAR --sim-type GEN -N 32768 -K 19661 --dec-snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K19661_SNR25.report 2>&1
../build/bin/aff3ct --cde-type POLAR --sim-type GEN -N 32768 -K 22938 --dec-snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K22938_SNR25.report 2>&1
../build/bin/aff3ct --cde-type POLAR --sim-type GEN -N 32768 -K 26214 --dec-snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K26214_SNR25.report 2>&1
../build/bin/aff3ct --cde-type POLAR --sim-type GEN -N 32768 -K 29491 --dec-snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K29491_SNR25.report 2>&1

echo "Generate decoders (other rates)..."
../build/bin/aff3ct --cde-type POLAR --sim-type GEN -N  2048 -K   410 --dec-snr 2.5 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K410_SNR25.report 2>&1
../build/bin/aff3ct --cde-type POLAR --sim-type GEN -N  2048 -K  1843 --dec-snr 4.0 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N2048_K1843_SNR40.report 2>&1
../build/bin/aff3ct --cde-type POLAR --sim-type GEN -N 16384 -K 14746 --dec-snr 4.0 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N16384_K14746_SNR40.report 2>&1
../build/bin/aff3ct --cde-type POLAR --sim-type GEN -N 32768 -K 27568 --dec-snr 4.0 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K27568_SNR40.report 2>&1
../build/bin/aff3ct --cde-type POLAR --sim-type GEN -N 32768 -K 29492 --dec-snr 4.0 > ../src/Module/Decoder/Polar/SC/Generated/Decoder_polar_SC_fast_sys_N32768_K29492_SNR40.report 2>&1

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
