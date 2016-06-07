#!/bin/bash

cd ../build

./bin/aff3ct                \
    --simu-type       BFER  \
    --code-type       POLAR \
    -K                128   \
    -N                256   \
    --snr-min         0.50  \
    --snr-max         4.01  \
    --snr-step        0.1   \
    --max-fe          100   \
    --dec-algo        SC    \
    --dec-implem      NAIVE \
    --domain          LLR   \
    $1 $2 $3 $4 $5 $6 $7 $8 $9 $10
