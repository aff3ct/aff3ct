#!/bin/bash
cd test

AFF3CT_PATH=../../../build/bin/aff3ct

$AFF3CT_PATH -p "8" --sim-type "BFER" -C "POLAR" -K "32" -N "64" -m "2.81"     \
-M "2.81" -e "2" --enc-fb-gen-method "GA" --dec-type "SC"                      \
--src-type "RAND_FAST" --chn-type "AWGN" --chn-implem "FAST"                   \
--dec-implem "FAST" --dec-simd "INTRA" --sim-debug --sim-pyber "test dump"     \
> dump_debug_gold.txt

$AFF3CT_PATH --sim-type "BFER" -p "8" --sim-cde-type "UNCODED" -K "12" -m "5"  \
-M "5" -s "0.5" --chn-type "RAYLEIGH" -t "1" --mdm-type "SCMA" --mdm-ite "4"   \
--mnt-max-fe "100" -F "6" --sim-debug --sim-pyber "Dump Test"                  \
> dump_debug_gold2.txt

$AFF3CT_PATH --sim-type "BFER" -p "8" --sim-cde-type "UNCODED" -K "12" -m "5"  \
-M "5" -s "0.5" --chn-type "RAYLEIGH" -t "1" --mdm-type "SCMA" --mdm-ite "4"   \
--mnt-max-fe "100" -F "6" --sim-debug-hex --sim-pyber "Dump Test"              \
> dump_debug_gold4.txt


echo "Test 1..."
rm -f Y_N.txt V_K.txt Y_N.bin V_K.bin Y_N.h V_K.h
python3 ../aff3ct_debug_parser.py ./dump_debug_gold.txt                        \
--mod Decoder_polar_SC_fast_sys --tsk decode_siho --src --txt --bin
diff -s Y_N.txt Y_N_gold.txt
diff -s V_K.txt V_K_gold.txt
diff -s Y_N.bin Y_N_gold.bin
diff -s V_K.bin V_K_gold.bin
diff -s Y_N.h Y_N_gold.h
diff -s V_K.h V_K_gold.h
rm -f Y_N.txt V_K.txt Y_N.bin V_K.bin Y_N.h V_K.h
echo "Test 1 Done."

echo "Test 2..."
rm -f Y_N.txt X_N.txt H_N.txt Y_N.bin X_N.bin H_N.bin Y_N.h X_N.h H_N.h
python3 ../aff3ct_debug_parser.py ./dump_debug_gold2.txt                       \
--mod Channel_Rayleigh_LLR --tsk add_noise_wg --src --txt --bin
diff -s Y_N.txt Y_N_gold2.txt
diff -s X_N.txt X_N_gold2.txt
diff -s H_N.txt H_N_gold2.txt
diff -s Y_N.bin Y_N_gold2.bin
diff -s X_N.bin X_N_gold2.bin
diff -s H_N.bin H_N_gold2.bin
diff -s Y_N.h Y_N_gold2.h
diff -s X_N.h X_N_gold2.h
diff -s H_N.h H_N_gold2.h
rm -f Y_N.txt X_N.txt H_N.txt Y_N.bin X_N.bin H_N.bin Y_N.h X_N.h H_N.h
echo "Test 2 Done."

echo "Test 3..."
rm -f Y_N1.txt Y_N2.txt Y_N1.bin Y_N2.bin Y_N1.h Y_N2.h
python3 ../aff3ct_debug_parser.py ./dump_debug_gold.txt                        \
--mod Quantizer_standard --tsk process --src --txt --bin
diff -s Y_N1.txt Y_N1_gold3.txt
diff -s Y_N2.txt Y_N2_gold3.txt
diff -s Y_N1.bin Y_N1_gold3.bin
diff -s Y_N2.bin Y_N2_gold3.bin
diff -s Y_N1.h Y_N1_gold3.h
diff -s Y_N2.h Y_N2_gold3.h
rm -f Y_N1.txt Y_N2.txt Y_N1.bin Y_N2.bin Y_N1.h Y_N2.h
echo "Test 3 Done."

echo "Test 4..."
rm -f Y_N1.txt Y_N2.txt Y_N1.bin Y_N2.bin Y_N1.h Y_N2.h
python3 ../aff3ct_debug_parser.py ./dump_debug_gold4.txt                        \
--mod Quantizer_standard --tsk process --src --txt --bin --fra 3
diff -s Y_N1.txt Y_N1_gold4.txt
diff -s Y_N2.txt Y_N2_gold4.txt
diff -s Y_N1.bin Y_N1_gold4.bin
diff -s Y_N2.bin Y_N2_gold4.bin
diff -s Y_N1.h Y_N1_gold4.h
diff -s Y_N2.h Y_N2_gold4.h
rm -f Y_N1.txt Y_N2.txt Y_N1.bin Y_N2.bin Y_N1.h Y_N2.h

rm dump_debug_gold.txt dump_debug_gold2.txt dump_debug_gold4.txt
echo "Test 4 Done."

cd - > /dev/null
