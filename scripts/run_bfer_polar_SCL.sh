#!/bin/bash

cd ../build

./bin/aff3ct                        \
    --simu-type       BFER          \
    --code-type       POLAR         \
    -K                344           \
    -N                1024          \
    --snr-min         1.0           \
    --snr-max         2.01          \
    --snr-step        0.25          \
    --max-fe          100           \
    --dec-algo        SCL           \
    --dec-implem      NAIVE         \
    -L                4             \
    --domain          LLR           \
    --crc-type        CRC-8-DVB-S2  \
    $1 $2 $3 $4 $5 $6 $7 $8 $9 $10

