#!/bin/bash

cd ../build

./bin/aff3ct                     \
    --simu-type       BFER       \
    --code-type       POLAR      \
    -K                4          \
    -N                8          \
    --snr-min         0.50       \
    --snr-max         0.50       \
    --snr-step        0.1        \
    --max-fe          1          \
    --enable-debug               \
    --dec-algo        SC         \
    --dec-implem      NAIVE      \
    --domain          LLR        \
    $1 $2 $3 $4 $5 $6 $7 $8 $9 $10
