#!/bin/bash

cd ../build

./bin/aff3ct                     \
    --sim-type        BFER       \
    --cde-type        POLAR      \
    -K                4          \
    -N                8          \
    --sim-snr-min     0.50       \
    --sim-snr-max     0.50       \
    --sim-snr-step    0.1        \
    --mnt-max-fe      1          \
    --sim-debug                  \
    --dec-type        SC         \
    --dec-implem      NAIVE      \
    $1 $2 $3 $4 $5 $6 $7 $8 $9 $10
