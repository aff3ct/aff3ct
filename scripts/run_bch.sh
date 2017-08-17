#!/bin/bash

cd ../build

./bin/aff3ct            \
    --sim-type     BFER \
    --sim-cde-type BCH  \
    -N             63   \
    -K             57   \
    -t             1    \
    --sim-snr-min  0    \
    --sim-snr-max  9    \
    --sim-snr-step 0.5  \
    "$@"
