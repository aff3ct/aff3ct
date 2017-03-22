#!/bin/bash

cd ../build

./bin/aff3ct                     \
    --sim-type       EXIT        \
    --cde-type       POLAR       \
    -K               1194        \
    -N               2048        \
    --sim-snr-min    -3          \
    --sim-snr-max    -3          \
    --sim-siga-min   0.0         \
    --sim-siga-max   5.01        \
    --sim-snr-step   0.1         \
    --mnt-max-fe     100         \
    --chn-type       AWGN        \
    --dec-type       SCAN        \
    --dec-implem     NAIVE       \
    --cde-sigma      0.8         \
    $*
