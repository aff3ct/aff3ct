#!/bin/bash

cd ../build

./aff3ct					 \
	--sim-type	   BFERI	 \
    --sim-prec     32        \
	--sim-snr-min  0.0		 \
	--sim-snr-max  2.5		 \
	--sim-snr-step 0.25		 \
	--mnt-max-fe   100		 \
    --src-type     RAND_FAST \
    -K             2112      \
    --cde-type     LDPC      \
    --cde-coset              \
    -N             4224      \
    --itl-type     USER      \
    --itl-path     ../conf/itl/GSM-LDPC_4224.itl \
    --mod-type     CPM       \
    --dmod-max     MAXS      \
    --chn-type     AWGN_FAST \
    --dec-type     BP_FLOODING\
    --dec-implem   SPA       \
    --cde-alist-path ../conf/dec/LDPC/GSM_2112_4224.alist \
    --mod-cpm-std  GSM       \
	$*
