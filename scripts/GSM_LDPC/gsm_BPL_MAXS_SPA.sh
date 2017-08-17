#!/bin/bash

cd ../../build

./bin/aff3ct                                            \
    --sim-type     BFERI                                \
    --sim-prec     32                                   \
    --sim-snr-min  1.0                                  \
    --sim-snr-max  3.0                                  \
    --sim-snr-step 0.1                                  \
    --mnt-max-fe   100                                  \
    --src-type     RAND_FAST                            \
    -K             2112                                 \
    --sim-cde-type LDPC                                 \
    --sim-coset                                         \
    -N             4224                                 \
    --itl-type     RANDOM                               \
    --mdm-type     CPM                                  \
    --mdm-max      MAXS                                 \
    --chn-type     AWGN_FAST                            \
    --dec-type     BP_LAYERED                           \
    --dec-implem   SPA                                  \
    --dec-no-synd                                       \
    --dec-h-path   ../conf/dec/LDPC/GSM_2112_4224.alist \
    --mdm-cpm-std  GSM                                  \
    "$@"
