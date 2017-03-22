#!/bin/bash

cd ../build

./bin/aff3ct                     \
    --sim-type        BFER       \
    --cde-type        POLAR      \
    -K                128        \
    -N                256        \
    --sim-snr-min     0.50       \
    --sim-snr-max     4.01       \
    --sim-snr-step    0.1        \
    --mnt-max-fe      100        \
    --dec-type        SC         \
    --dec-implem      NAIVE      \
    $*
