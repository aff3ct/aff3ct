#!/bin/bash

cd ../build

./bin/aff3ct           \
    --simu-type  BFER  \
    --code-type  TURBO \
    -K           6144  \
    -N           18432 \
    --snr-min    0.0   \
    --snr-max    1.01  \
    --snr-step   0.1   \
    --max-fe     100   \
    --max-iter   6     \
    --dec-algo   LTE   \
    --dec-implem FAST  \
    --domain     LLR   \
    $1 $2 $3 $4 $5 $6 $7 $8 $9 $10
