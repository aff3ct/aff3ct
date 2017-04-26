#!/bin/bash

cd ../build

./bin/aff3ct                    \
    --simu-type       BFER      \
    --code-type       BCH       \
    --mGF              6        \
    -N                63        \
    -K                57        \
    -t                1         \
    --snr-min         0         \
    --snr-max         9         \
    --snr-step        0.5       \
    --decoding-algo   ALGEBRAIC \
    $1 $2 $3 $4 $5 $6 $7 $8 $9 $10
