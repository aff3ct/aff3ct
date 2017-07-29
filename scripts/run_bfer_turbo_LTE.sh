#!/bin/bash

cd ../build

./bin/aff3ct             \
    --sim-type     BFER  \
    --sim-cde-type TURBO \
    -K             6144  \
    --sim-snr-min  0.0   \
    --sim-snr-max  1.01  \
    --sim-snr-step 0.1   \
    --mnt-max-fe   100   \
    --enc-std      LTE   \
    --dec-ite      6     \
    --dec-type     BCJR  \
    --dec-implem   FAST  \
    "$@"
