#!/bin/bash

cd ../build

./bin/aff3ct                     \
    --sim-type        BFER       \
    --cde-type        POLAR      \
    -K                344        \
    -N                1024       \
    --sim-snr-min     1.0        \
    --sim-snr-max     2.01       \
    --sim-snr-step    0.25       \
    --mnt-max-fe      100        \
    --dec-type        SCL        \
    --dec-implem      NAIVE      \
    -L                4          \
    --crc-type        8-DVB-S2   \
    $1 $2 $3 $4 $5 $6 $7 $8 $9 $10

