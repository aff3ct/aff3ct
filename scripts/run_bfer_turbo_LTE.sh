#!/bin/bash

cd ../build

./bin/aff3ct                     \
    --sim-type       BFER        \
    --cde-type       TURBO       \
    -K               6144        \
    -N               18432       \
    --sim-snr-min    0.0         \
    --sim-snr-max    1.01        \
    --sim-snr-step   0.1         \
    --mnt-max-fe     100         \
    --dec-ite        6           \
    --dec-type       LTE         \
    --dec-implem     FAST        \
    $1 $2 $3 $4 $5 $6 $7 $8 $9 $10
