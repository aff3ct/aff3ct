# AFF3CT: A Fast Forward Error Correction Toolbox!

[![GitLab Pipeline Status](https://img.shields.io/gitlab/pipeline/aff3ct/aff3ct.svg)](https://gitlab.com/aff3ct/aff3ct/pipelines)
[![SonarQube Code Coverage](https://gitlab.com/aff3ct/aff3ct/badges/master/coverage.svg)](https://sonarqube.inria.fr/sonarqube/dashboard?id=storm%3Aaff3ct%3Agitlab%3Amaster)
[![Documentation Build Status](https://img.shields.io/readthedocs/aff3ct.svg)](https://readthedocs.org/projects/aff3ct/)
[![Latest Release](https://img.shields.io/github/release/aff3ct/aff3ct.svg)](https://github.com/aff3ct/aff3ct/releases)
[![DOI](https://zenodo.org/badge/60615913.svg)](https://zenodo.org/badge/latestdoi/60615913)
[![License: MIT](https://img.shields.io/github/license/aff3ct/aff3ct.svg)](./LICENSE)

**AFF3CT** is a simulator dedicated to the Forward Error Correction (FEC or
**channel coding**). It is written in **C++** and it supports a large range of
codes: from the well-spread **Turbo codes** to the very new **Polar codes**
including the **Low-Density Parity-Check (LDPC) codes**. **AFF3CT** is a command
line program and it simulates communication chains based on a Monte Carlo
method.

It is very easy to use, for instance, to estimate the BER/FER decoding
performances of the (2048,1723) Polar code from 1.0 to 4.0 dB:

```bash
aff3ct -C "POLAR" -K 1723 -N 2048 -m 1.0 -M 4.0 -s 1.0
```

And the output will be:

```bash
# ----------------------------------------------------
# ---- A FAST FORWARD ERROR CORRECTION TOOLBOX >> ----
# ----------------------------------------------------
# Parameters :
# [...]
#
# The simulation is running...
# ---------------------||------------------------------------------------------||---------------------
#  Signal Noise Ratio  ||   Bit Error Rate (BER) and Frame Error Rate (FER)    ||  Global throughput
#         (SNR)        ||                                                      ||  and elapsed time
# ---------------------||------------------------------------------------------||---------------------
# ----------|----------||----------|----------|----------|----------|----------||----------|----------
#     Es/N0 |    Eb/N0 ||      FRA |       BE |       FE |      BER |      FER ||  SIM_THR |    ET/RT
#      (dB) |     (dB) ||          |          |          |          |          ||   (Mb/s) | (hhmmss)
# ----------|----------||----------|----------|----------|----------|----------||----------|----------
       0.25 |     1.00 ||      104 |    16425 |      104 | 9.17e-02 | 1.00e+00 ||    4.995 | 00h00'00
       1.25 |     2.00 ||      104 |    12285 |      104 | 6.86e-02 | 1.00e+00 ||   13.678 | 00h00'00
       2.25 |     3.00 ||      147 |     5600 |      102 | 2.21e-02 | 6.94e-01 ||   14.301 | 00h00'00
       3.25 |     4.00 ||     5055 |     2769 |      100 | 3.18e-04 | 1.98e-02 ||   30.382 | 00h00'00
# End of the simulation.
```

## Features

**The simulator targets high speed simulations** and extensively uses parallel
techniques like SIMD, multi-threading and multi-nodes programming models.
Below, a list of the features that motivated the creation of the simulator:

1. **reproduce state-of-the-art decoding performances**,
2. **explore various channel code configurations**, find new trade-offs,
3. **prototype hardware implementation** (fixed-point receivers, hardware in
   the loop tools),
4. **reuse tried and tested modules** and add yours,
5. **alternative to MATLAB**, if you seek to reduce simulations time.

## Installation

First make sure to have installed a C++11 compiler, CMake and Git. Then install
AFF3CT by running:

```bash
git clone --recursive https://github.com/aff3ct/aff3ct.git
mkdir aff3ct/build
cd aff3ct/build
cmake .. -DCMAKE_BUILD_TYPE="Release"
make -j4
```

## Contribute

- [Source Code](https://github.com/aff3ct/aff3ct)
- [Contributing guidelines](./CONTRIBUTING.md)

## Support

If you are having issues, please let us know on our
[issue tracker](https://github.com/aff3ct/aff3ct/issues).

## License

The project is licensed under the MIT license.

External Links
--------------

- [Official website](https://aff3ct.github.io)
- [Documentation](https://aff3ct.readthedocs.io)
