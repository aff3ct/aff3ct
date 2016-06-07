#!/bin/bash

cd ../build

./bin/aff3ct             \
    --simu-type    EXIT  \
    --code-type    POLAR \
    -K             1194  \
    -N             2048  \
    --snr-min      -3    \
    --snr-max      -3    \
    --sig-a-min    0.0   \
    --sig-a-max    5.01  \
    --snr-step     0.1   \
    --max-fe       100   \
    --channel-type AWGN  \
    --dec-algo     SCAN  \
    --dec-implem   NAIVE \
    --domain       LLR   \
    --code-sigma   0.8   \
    $1 $2 $3 $4 $5 $6 $7 $8 $9 $10
