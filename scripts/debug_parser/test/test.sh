#!/bin/bash
cd test

AFF3CT_PATH=../../../build/bin/aff3ct

$AFF3CT_PATH -p "8" --sim-type "BFER" -C "POLAR" -K "32" -N "64" -m "2.81"     \
-M "2.81" -e "2" --enc-fb-gen-method "GA" --dec-type "SC"                      \
--src-type "RAND" --src-implem FAST --chn-type "AWGN" --chn-implem "FAST"                   \
--dec-implem "FAST" --dec-simd "INTRA" --sim-dbg --sim-meta "test dump"     \
> dump_debug_gold.txt

$AFF3CT_PATH --sim-type "BFER" -p "8" --sim-cde-type "UNCODED" -K "12" -m "5"  \
-M "5" -s "0.5" --chn-type "RAYLEIGH" -t "1" --mdm-type "SCMA" --mdm-ite "4"   \
--mnt-max-fe "100" -F "6" --sim-dbg --sim-meta "Dump Test"                  \
--mdm-codebook "conf/mod/SCMA/CS1.cb" \
> dump_debug_gold2.txt

$AFF3CT_PATH --sim-type "BFER" -p "8" --sim-cde-type "UNCODED" -K "12" -m "5"  \
-M "5" -s "0.5" --chn-type "RAYLEIGH" -t "1" --mdm-type "SCMA" --mdm-ite "4"   \
--mnt-max-fe "100" -F "6" --sim-dbg-hex --sim-meta "Dump Test"              \
--mdm-codebook "conf/mod/SCMA/CS1.cb" \
> dump_debug_gold4.txt


# Test 1...
rm -f Y_N.txt V_K.txt Y_N.bin V_K.bin Y_N.h V_K.h
python3 ../aff3ct_debug_parser.py ./dump_debug_gold.txt                        \
--mod Decoder_polar_SC_fast_sys --tsk decode_siho --src --txt --bin --mat
cmp Y_N.txt refs/Y_N_gold.txt
cmp V_K.txt refs/V_K_gold.txt
cmp Y_N.bin refs/Y_N_gold.bin
cmp V_K.bin refs/V_K_gold.bin
cmp Y_N.h   refs/Y_N_gold.h
cmp V_K.h   refs/V_K_gold.h
cmp Y_N.h   refs/Y_N_gold.h
cmp V_K.h   refs/V_K_gold.h
cmp Y_N.mat     refs/Y_N_gold.mat -i 70
cmp V_K.mat     refs/V_K_gold.mat -i 70
rm -f Y_N.txt V_K.txt Y_N.bin V_K.bin Y_N.h V_K.h
rm -f Y_N.mat V_K.mat 
# Test 1 Done.

# Test 2...
rm -f Y_N.txt X_N.txt H_N.txt Y_N.bin X_N.bin H_N.bin Y_N.h X_N.h H_N.h
python3 ../aff3ct_debug_parser.py ./dump_debug_gold2.txt                       \
--mod Channel_Rayleigh_LLR --tsk add_noise_wg --src --txt --bin --mat
cmp Y_N.txt refs/Y_N_gold2.txt
cmp X_N.txt refs/X_N_gold2.txt
cmp H_N.txt refs/H_N_gold2.txt
cmp Y_N.bin refs/Y_N_gold2.bin
cmp X_N.bin refs/X_N_gold2.bin
cmp H_N.bin refs/H_N_gold2.bin
cmp Y_N.h   refs/Y_N_gold2.h
cmp X_N.h   refs/X_N_gold2.h
cmp H_N.h   refs/H_N_gold2.h
cmp Y_N.mat     refs/Y_N_gold2.mat -i 70
cmp X_N.mat     refs/X_N_gold2.mat -i 70
cmp H_N.mat     refs/H_N_gold2.mat -i 70
rm -f Y_N.txt X_N.txt H_N.txt Y_N.bin X_N.bin H_N.bin Y_N.h X_N.h H_N.h
rm -f Y_N.mat X_N.mat H_N.mat
# Test 2 Done.

# Test 3...
rm -f Y_N1.txt Y_N2.txt Y_N1.bin Y_N2.bin Y_N1.h Y_N2.h
python3 ../aff3ct_debug_parser.py ./dump_debug_gold.txt                        \
--mod Quantizer_pow2 --tsk process --src --txt --bin --mat
cmp Y_N1.txt refs/Y_N1_gold3.txt
cmp Y_N2.txt refs/Y_N2_gold3.txt
cmp Y_N1.bin refs/Y_N1_gold3.bin
cmp Y_N2.bin refs/Y_N2_gold3.bin
cmp Y_N1.h   refs/Y_N1_gold3.h
cmp Y_N2.h   refs/Y_N2_gold3.h
cmp Y_N1.mat     refs/Y_N1_gold3.mat -i 70
cmp Y_N2.mat     refs/Y_N2_gold3.mat -i 70
rm -f Y_N1.txt Y_N2.txt Y_N1.bin Y_N2.bin Y_N1.h Y_N2.h
rm -f Y_N1.mat Y_N2.mat
# Test 3 Done.

# Test 4...
rm -f Y_N1.txt Y_N2.txt Y_N1.bin Y_N2.bin Y_N1.h Y_N2.h
python3 ../aff3ct_debug_parser.py ./dump_debug_gold4.txt                        \
--mod Quantizer_pow2 --tsk process --src --txt --bin --mat --fra 3
cmp Y_N1.txt refs/Y_N1_gold4.txt
cmp Y_N2.txt refs/Y_N2_gold4.txt
cmp Y_N1.bin refs/Y_N1_gold4.bin
cmp Y_N2.bin refs/Y_N2_gold4.bin
cmp Y_N1.h   refs/Y_N1_gold4.h
cmp Y_N2.h   refs/Y_N2_gold4.h
cmp Y_N1.mat     refs/Y_N1_gold4.mat -i 70
cmp Y_N2.mat     refs/Y_N2_gold4.mat -i 70
rm -f Y_N1.txt Y_N2.txt Y_N1.bin Y_N2.bin Y_N1.h Y_N2.h
rm -f Y_N1.mat Y_N2.mat
# Test 4 Done.

rm dump_debug_gold.txt dump_debug_gold2.txt dump_debug_gold4.txt

cd - > /dev/null
