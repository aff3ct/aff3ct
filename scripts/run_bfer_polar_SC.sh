#!/bin/bash

cd ../build

./bin/aff3ct                     \
    --sim-type     BFER          \
    --cde-type     POLAR         \
    -K             2048          \
    -N             4096          \
    --sim-snr-min  2.0           \
    --sim-snr-max  3.01          \
    --sim-snr-step 0.1           \
    --mnt-max-fe   100           \
    --dec-type     SC            \
    --dec-implem   FAST          \
    $1 $2 $3 $4 $5 $6 $7 $8 $9 $10
