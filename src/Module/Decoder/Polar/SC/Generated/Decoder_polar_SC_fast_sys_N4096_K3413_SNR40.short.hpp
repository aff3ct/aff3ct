#ifndef DECODER_POLAR_SC_FAST_SYS_N4096_K3413_SNR40_HPP_
#define DECODER_POLAR_SC_FAST_SYS_N4096_K3413_SNR40_HPP_

#include <cassert>

#include "../Decoder_polar_SC_fast_sys.hpp"

namespace aff3ct
{
namespace module
{
static const char Frozen_bits_4096_3413_40[4096] = {
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

template <typename B, typename R, class API_polar>
class Decoder_polar_SC_fast_sys_N4096_K3413_SNR40 : public Decoder_polar_SC_fast_sys<B, R, API_polar>
{
public:
	Decoder_polar_SC_fast_sys_N4096_K3413_SNR40(const int& K, const int& N, const mipp::vector<B>& frozen_bits, const int n_frames = 1)
	: Decoder_polar_SC_fast_sys<B, R, API_polar>(K, N, frozen_bits, n_frames)
	{
		assert(N == 4096);
		assert(K == 3413);
		
		auto i = 0;
		while (i < 4096 && Frozen_bits_4096_3413_40[i] == frozen_bits[i]) i++;
		assert(i == 4096);
	}

	virtual ~Decoder_polar_SC_fast_sys_N4096_K3413_SNR40()
	{
	}

	// depth = 8, reverse depth = 4, size = 16, calls = 3
	inline void r08(const int off_l, const int off_s)
	{
	}

	// depth = 9, reverse depth = 3, size = 8, calls = 3
	__attribute__((always_inline))
	inline void r09(const int off_l, const int off_s)
	{
	}

	// depth = 10, reverse depth = 2, size = 4, calls = 27
	__attribute__((always_inline))
	inline void re10(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template rep<   4>(s, l, off_l+   0,                       off_s+   0,    4);
	}

	// depth = 10, reverse depth = 2, size = 4, calls = 28
	__attribute__((always_inline))
	inline void s10(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template spc<   4>(s, l, off_l+   0,                       off_s+   0,    4);
	}

	// depth = 9, reverse depth = 3, size = 8, calls = 24
	__attribute__((always_inline))
	inline void re10s10(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <   4>(   l, off_l+   0, off_l+   4,            off_l+   8,    4);
		re10(off_l+8, off_s+0);
		API_polar::template gr <   4>(s, l, off_l+   0, off_l+   4, off_s+   0, off_l+   8,    4);
		s10(off_l+8, off_s+4);
		API_polar::template xo <   4>(s,    off_s+   0, off_s+   4,            off_s+   0,    4);
	}

	// depth = 8, reverse depth = 4, size = 16, calls = 2
	inline void r09re10s10(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		r09(off_l+16, off_s+0);
		API_polar::template g0 <   8>(   l, off_l+   0, off_l+   8,            off_l+  16,    8);
		re10s10(off_l+16, off_s+8);
		API_polar::template xo0<   8>(s,    off_s+   8,                       off_s+   0,    8);
	}

	// depth = 9, reverse depth = 3, size = 8, calls = 12
	__attribute__((always_inline))
	inline void re9(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template rep<   8>(s, l, off_l+   0,                       off_s+   0,    8);
	}

	// depth = 8, reverse depth = 4, size = 16, calls = 8
	inline void re9re10s10(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <   8>(   l, off_l+   0, off_l+   8,            off_l+  16,    8);
		re9(off_l+16, off_s+0);
		API_polar::template gr <   8>(s, l, off_l+   0, off_l+   8, off_s+   0, off_l+  16,    8);
		re10s10(off_l+16, off_s+8);
		API_polar::template xo <   8>(s,    off_s+   0, off_s+   8,            off_s+   0,    8);
	}

	// depth = 9, reverse depth = 3, size = 8, calls = 16
	__attribute__((always_inline))
	inline void s9(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template spc<   8>(s, l, off_l+   0,                       off_s+   0,    8);
	}

	// depth = 8, reverse depth = 4, size = 16, calls = 12
	inline void re10s10s9(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <   8>(   l, off_l+   0, off_l+   8,            off_l+  16,    8);
		re10s10(off_l+16, off_s+0);
		API_polar::template g  <   8>(s, l, off_l+   0, off_l+   8, off_s+   0, off_l+  16,    8);
		s9(off_l+16, off_s+8);
		API_polar::template xo <   8>(s,    off_s+   0, off_s+   8,            off_s+   0,    8);
	}

	// depth = 7, reverse depth = 5, size = 32, calls = 3
	inline void re9re10s10re10s10s9(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <  16>(   l, off_l+   0, off_l+  16,            off_l+  32,   16);
		re9re10s10(off_l+32, off_s+0);
		API_polar::template g  <  16>(s, l, off_l+   0, off_l+  16, off_s+   0, off_l+  32,   16);
		re10s10s9(off_l+32, off_s+16);
		API_polar::template xo <  16>(s,    off_s+   0, off_s+  16,            off_s+   0,   16);
	}

	// depth = 7, reverse depth = 5, size = 32, calls = 2
	inline void r08re9re10s10(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		r08(off_l+32, off_s+0);
		API_polar::template g0 <  16>(   l, off_l+   0, off_l+  16,            off_l+  32,   16);
		re9re10s10(off_l+32, off_s+16);
		API_polar::template xo0<  16>(s,    off_s+  16,                       off_s+   0,   16);
	}

	// depth = 8, reverse depth = 4, size = 16, calls = 4
	inline void re8(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template rep<  16>(s, l, off_l+   0,                       off_s+   0,   16);
	}

	// depth = 11, reverse depth = 1, size = 2, calls = 5
	__attribute__((always_inline))
	inline void r011(const int off_l, const int off_s)
	{
	}

	// depth = 11, reverse depth = 1, size = 2, calls = 5
	__attribute__((always_inline))
	inline void r111(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template h  <   2>(s, l, off_l+   0,                       off_s+   0,    2);
	}

	// depth = 10, reverse depth = 2, size = 4, calls = 5
	__attribute__((always_inline))
	inline void r011r111(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		r011(off_l+4, off_s+0);
		API_polar::template g0 <   2>(   l, off_l+   0, off_l+   2,            off_l+   4,    2);
		r111(off_l+4, off_s+2);
		API_polar::template xo0<   2>(s,    off_s+   2,                       off_s+   0,    2);
	}

	// depth = 10, reverse depth = 2, size = 4, calls = 7
	__attribute__((always_inline))
	inline void r110(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template h  <   4>(s, l, off_l+   0,                       off_s+   0,    4);
	}

	// depth = 9, reverse depth = 3, size = 8, calls = 4
	__attribute__((always_inline))
	inline void r011r111r110(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <   4>(   l, off_l+   0, off_l+   4,            off_l+   8,    4);
		r011r111(off_l+8, off_s+0);
		API_polar::template g  <   4>(s, l, off_l+   0, off_l+   4, off_s+   0, off_l+   8,    4);
		r110(off_l+8, off_s+4);
		API_polar::template xo <   4>(s,    off_s+   0, off_s+   4,            off_s+   0,    4);
	}

	// depth = 8, reverse depth = 4, size = 16, calls = 4
	inline void re9r011r111r110(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <   8>(   l, off_l+   0, off_l+   8,            off_l+  16,    8);
		re9(off_l+16, off_s+0);
		API_polar::template gr <   8>(s, l, off_l+   0, off_l+   8, off_s+   0, off_l+  16,    8);
		r011r111r110(off_l+16, off_s+8);
		API_polar::template xo <   8>(s,    off_s+   0, off_s+   8,            off_s+   0,    8);
	}

	// depth = 8, reverse depth = 4, size = 16, calls = 11
	inline void s8(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template spc<  16>(s, l, off_l+   0,                       off_s+   0,   16);
	}

	// depth = 7, reverse depth = 5, size = 32, calls = 3
	inline void re9r011r111r110s8(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <  16>(   l, off_l+   0, off_l+  16,            off_l+  32,   16);
		re9r011r111r110(off_l+32, off_s+0);
		API_polar::template g  <  16>(s, l, off_l+   0, off_l+  16, off_s+   0, off_l+  32,   16);
		s8(off_l+32, off_s+16);
		API_polar::template xo <  16>(s,    off_s+   0, off_s+  16,            off_s+   0,   16);
	}

	// depth = 10, reverse depth = 2, size = 4, calls = 5
	__attribute__((always_inline))
	inline void r010(const int off_l, const int off_s)
	{
	}

	// depth = 7, reverse depth = 5, size = 32, calls = 6
	inline void re10s10s9s8(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <  16>(   l, off_l+   0, off_l+  16,            off_l+  32,   16);
		re10s10s9(off_l+32, off_s+0);
		API_polar::template g  <  16>(s, l, off_l+   0, off_l+  16, off_s+   0, off_l+  32,   16);
		s8(off_l+32, off_s+16);
		API_polar::template xo <  16>(s,    off_s+   0, off_s+  16,            off_s+   0,   16);
	}

	// depth = 7, reverse depth = 5, size = 32, calls = 8
	inline void s7(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template spc<  32>(s, l, off_l+   0,                       off_s+   0,   32);
	}

	// depth = 6, reverse depth = 6, size = 64, calls = 3
	inline void re10s10s9s8s7(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <  32>(   l, off_l+   0, off_l+  32,            off_l+  64,   32);
		re10s10s9s8(off_l+64, off_s+0);
		API_polar::template g  <  32>(s, l, off_l+   0, off_l+  32, off_s+   0, off_l+  64,   32);
		s7(off_l+64, off_s+32);
		API_polar::template xo <  32>(s,    off_s+   0, off_s+  32,            off_s+   0,   32);
	}

	// depth = 7, reverse depth = 5, size = 32, calls = 2
	inline void re7(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template rep<  32>(s, l, off_l+   0,                       off_s+   0,   32);
	}

	// depth = 9, reverse depth = 3, size = 8, calls = 4
	__attribute__((always_inline))
	inline void r010s10(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		r010(off_l+8, off_s+0);
		API_polar::template g0 <   4>(   l, off_l+   0, off_l+   4,            off_l+   8,    4);
		s10(off_l+8, off_s+4);
		API_polar::template xo0<   4>(s,    off_s+   4,                       off_s+   0,    4);
	}

	// depth = 8, reverse depth = 4, size = 16, calls = 3
	inline void r010s10s9(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <   8>(   l, off_l+   0, off_l+   8,            off_l+  16,    8);
		r010s10(off_l+16, off_s+0);
		API_polar::template g  <   8>(s, l, off_l+   0, off_l+   8, off_s+   0, off_l+  16,    8);
		s9(off_l+16, off_s+8);
		API_polar::template xo <   8>(s,    off_s+   0, off_s+   8,            off_s+   0,    8);
	}

	// depth = 9, reverse depth = 3, size = 8, calls = 5
	__attribute__((always_inline))
	inline void r19(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template h  <   8>(s, l, off_l+   0,                       off_s+   0,    8);
	}

	// depth = 8, reverse depth = 4, size = 16, calls = 2
	inline void re10s10r19(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <   8>(   l, off_l+   0, off_l+   8,            off_l+  16,    8);
		re10s10(off_l+16, off_s+0);
		API_polar::template g  <   8>(s, l, off_l+   0, off_l+   8, off_s+   0, off_l+  16,    8);
		r19(off_l+16, off_s+8);
		API_polar::template xo <   8>(s,    off_s+   0, off_s+   8,            off_s+   0,    8);
	}

	// depth = 8, reverse depth = 4, size = 16, calls = 4
	inline void r18(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template h  <  16>(s, l, off_l+   0,                       off_s+   0,   16);
	}

	// depth = 7, reverse depth = 5, size = 32, calls = 2
	inline void re10s10r19r18(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <  16>(   l, off_l+   0, off_l+  16,            off_l+  32,   16);
		re10s10r19(off_l+32, off_s+0);
		API_polar::template g  <  16>(s, l, off_l+   0, off_l+  16, off_s+   0, off_l+  32,   16);
		r18(off_l+32, off_s+16);
		API_polar::template xo <  16>(s,    off_s+   0, off_s+  16,            off_s+   0,   16);
	}

	// depth = 7, reverse depth = 5, size = 32, calls = 2
	inline void r17(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template h  <  32>(s, l, off_l+   0,                       off_s+   0,   32);
	}

	// depth = 9, reverse depth = 3, size = 8, calls = 3
	__attribute__((always_inline))
	inline void re10r110(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <   4>(   l, off_l+   0, off_l+   4,            off_l+   8,    4);
		re10(off_l+8, off_s+0);
		API_polar::template gr <   4>(s, l, off_l+   0, off_l+   4, off_s+   0, off_l+   8,    4);
		r110(off_l+8, off_s+4);
		API_polar::template xo <   4>(s,    off_s+   0, off_s+   4,            off_s+   0,    4);
	}

	// depth = 8, reverse depth = 4, size = 16, calls = 3
	inline void re10r110r19(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <   8>(   l, off_l+   0, off_l+   8,            off_l+  16,    8);
		re10r110(off_l+16, off_s+0);
		API_polar::template g  <   8>(s, l, off_l+   0, off_l+   8, off_s+   0, off_l+  16,    8);
		r19(off_l+16, off_s+8);
		API_polar::template xo <   8>(s,    off_s+   0, off_s+   8,            off_s+   0,    8);
	}

	// depth = 6, reverse depth = 6, size = 64, calls = 6
	inline void s6(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template spc<  64>(s, l, off_l+   0,                       off_s+   0,   64);
	}

	// depth = 5, reverse depth = 7, size = 128, calls = 3
	inline void s5(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template spc< 128>(s, l, off_l+   0,                       off_s+   0,  128);
	}

	// depth = 7, reverse depth = 5, size = 32, calls = 2
	inline void re9re10s10re10r110r19(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <  16>(   l, off_l+   0, off_l+  16,            off_l+  32,   16);
		re9re10s10(off_l+32, off_s+0);
		API_polar::template g  <  16>(s, l, off_l+   0, off_l+  16, off_s+   0, off_l+  32,   16);
		re10r110r19(off_l+32, off_s+16);
		API_polar::template xo <  16>(s,    off_s+   0, off_s+  16,            off_s+   0,   16);
	}

	// depth = 6, reverse depth = 6, size = 64, calls = 2
	inline void re9r011r111r110s8s7(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <  32>(   l, off_l+   0, off_l+  32,            off_l+  64,   32);
		re9r011r111r110s8(off_l+64, off_s+0);
		API_polar::template g  <  32>(s, l, off_l+   0, off_l+  32, off_s+   0, off_l+  64,   32);
		s7(off_l+64, off_s+32);
		API_polar::template xo <  32>(s,    off_s+   0, off_s+  32,            off_s+   0,   32);
	}

	// depth = 7, reverse depth = 5, size = 32, calls = 2
	inline void r010s10s9s8(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <  16>(   l, off_l+   0, off_l+  16,            off_l+  32,   16);
		r010s10s9(off_l+32, off_s+0);
		API_polar::template g  <  16>(s, l, off_l+   0, off_l+  16, off_s+   0, off_l+  32,   16);
		s8(off_l+32, off_s+16);
		API_polar::template xo <  16>(s,    off_s+   0, off_s+  16,            off_s+   0,   16);
	}

	// depth = 6, reverse depth = 6, size = 64, calls = 2
	inline void r010s10s9s8s7(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <  32>(   l, off_l+   0, off_l+  32,            off_l+  64,   32);
		r010s10s9s8(off_l+64, off_s+0);
		API_polar::template g  <  32>(s, l, off_l+   0, off_l+  32, off_s+   0, off_l+  64,   32);
		s7(off_l+64, off_s+32);
		API_polar::template xo <  32>(s,    off_s+   0, off_s+  32,            off_s+   0,   32);
	}

	// depth = 5, reverse depth = 7, size = 128, calls = 2
	inline void r010s10s9s8s7s6(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <  64>(   l, off_l+   0, off_l+  64,            off_l+ 128,   64);
		r010s10s9s8s7(off_l+128, off_s+0);
		API_polar::template g  <  64>(s, l, off_l+   0, off_l+  64, off_s+   0, off_l+ 128,   64);
		s6(off_l+128, off_s+64);
		API_polar::template xo <  64>(s,    off_s+   0, off_s+  64,            off_s+   0,   64);
	}

	// depth = 4, reverse depth = 8, size = 256, calls = 3
	inline void r14(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template h  < 256>(s, l, off_l+   0,                       off_s+   0,  256);
	}

	// depth = 5, reverse depth = 7, size = 128, calls = 3
	inline void r15(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template h  < 128>(s, l, off_l+   0,                       off_s+   0,  128);
	}

	// depth = 6, reverse depth = 6, size = 64, calls = 2
	inline void r16(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template h  <  64>(s, l, off_l+   0,                       off_s+   0,   64);
	}

	void decode()
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <2048>(   l,    0+   0,    0+2048,               0+4096, 2048);
		API_polar::template f  <1024>(   l, 4096+   0, 4096+1024,            4096+2048, 1024);
		API_polar::template f  < 512>(   l, 6144+   0, 6144+ 512,            6144+1024,  512);
		API_polar::template f  < 256>(   l, 7168+   0, 7168+ 256,            7168+ 512,  256);
		API_polar::template f  < 128>(   l, 7680+   0, 7680+ 128,            7680+ 256,  128);
		API_polar::template rep< 128>(s, l, 7936+   0,                          0+   0,  128);
		API_polar::template gr < 128>(s, l, 7680+   0, 7680+ 128,    0+   0, 7680+ 256,  128);
		API_polar::template f  <  64>(   l, 7936+   0, 7936+  64,            7936+ 128,   64);
		API_polar::template rep<  64>(s, l, 8064+   0,                        128+   0,   64);
		API_polar::template gr <  64>(s, l, 7936+   0, 7936+  64,  128+   0, 7936+ 128,   64);
		API_polar::template f  <  32>(   l, 8064+   0, 8064+  32,            8064+  64,   32);
		r08(8160, 192);
		API_polar::template g0 <  16>(   l, 8128+   0, 8128+  16,            8128+  32,   16);
		r09re10s10(8160, 208);
		API_polar::template xo0<  16>(s,     192+  16,                        192+   0,   16);
		API_polar::template g  <  32>(s, l, 8064+   0, 8064+  32,  192+   0, 8064+  64,   32);
		re9re10s10re10s10s9(8128, 224);
		API_polar::template xo <  32>(s,     192+   0,  192+  32,             192+   0,   32);
		API_polar::template xo <  64>(s,     128+   0,  128+  64,             128+   0,   64);
		API_polar::template xo < 128>(s,       0+   0,    0+ 128,               0+   0,  128);
		API_polar::template g  < 256>(s, l, 7168+   0, 7168+ 256,    0+   0, 7168+ 512,  256);
		API_polar::template f  < 128>(   l, 7680+   0, 7680+ 128,            7680+ 256,  128);
		API_polar::template f  <  64>(   l, 7936+   0, 7936+  64,            7936+ 128,   64);
		API_polar::template g0 <  32>(   l, 8064+   0, 8064+  32,            8064+  64,   32);
		r08re9re10s10(8128, 288);
		API_polar::template xo0<  32>(s,     256+  32,                        256+   0,   32);
		API_polar::template g  <  64>(s, l, 7936+   0, 7936+  64,  256+   0, 7936+ 128,   64);
		API_polar::template f  <  32>(   l, 8064+   0, 8064+  32,            8064+  64,   32);
		API_polar::template f  <  16>(   l, 8128+   0, 8128+  16,            8128+  32,   16);
		re8(8160, 320);
		API_polar::template gr <  16>(s, l, 8128+   0, 8128+  16,  320+   0, 8128+  32,   16);
		re9re10s10(8160, 336);
		API_polar::template xo <  16>(s,     320+   0,  320+  16,             320+   0,   16);
		API_polar::template g  <  32>(s, l, 8064+   0, 8064+  32,  320+   0, 8064+  64,   32);
		re9r011r111r110s8(8128, 352);
		API_polar::template xo <  32>(s,     320+   0,  320+  32,             320+   0,   32);
		API_polar::template xo <  64>(s,     256+   0,  256+  64,             256+   0,   64);
		API_polar::template g  < 128>(s, l, 7680+   0, 7680+ 128,  256+   0, 7680+ 256,  128);
		API_polar::template f  <  64>(   l, 7936+   0, 7936+  64,            7936+ 128,   64);
		API_polar::template f  <  32>(   l, 8064+   0, 8064+  32,            8064+  64,   32);
		API_polar::template f  <  16>(   l, 8128+   0, 8128+  16,            8128+  32,   16);
		re8(8160, 384);
		API_polar::template gr <  16>(s, l, 8128+   0, 8128+  16,  384+   0, 8128+  32,   16);
		API_polar::template f  <   8>(   l, 8160+   0, 8160+   8,            8160+  16,    8);
		r010(8184, 400);
		API_polar::template g0 <   4>(   l, 8176+   0, 8176+   4,            8176+   8,    4);
		r011r111(8184, 404);
		API_polar::template xo0<   4>(s,     400+   4,                        400+   0,    4);
		API_polar::template g  <   8>(s, l, 8160+   0, 8160+   8,  400+   0, 8160+  16,    8);
		s9(8176, 408);
		API_polar::template xo <   8>(s,     400+   0,  400+   8,             400+   0,    8);
		API_polar::template xo <  16>(s,     384+   0,  384+  16,             384+   0,   16);
		API_polar::template g  <  32>(s, l, 8064+   0, 8064+  32,  384+   0, 8064+  64,   32);
		re10s10s9s8(8128, 416);
		API_polar::template xo <  32>(s,     384+   0,  384+  32,             384+   0,   32);
		API_polar::template g  <  64>(s, l, 7936+   0, 7936+  64,  384+   0, 7936+ 128,   64);
		re10s10s9s8s7(8064, 448);
		API_polar::template xo <  64>(s,     384+   0,  384+  64,             384+   0,   64);
		API_polar::template xo < 128>(s,     256+   0,  256+ 128,             256+   0,  128);
		API_polar::template xo < 256>(s,       0+   0,    0+ 256,               0+   0,  256);
		API_polar::template g  < 512>(s, l, 6144+   0, 6144+ 512,    0+   0, 6144+1024,  512);
		API_polar::template f  < 256>(   l, 7168+   0, 7168+ 256,            7168+ 512,  256);
		API_polar::template f  < 128>(   l, 7680+   0, 7680+ 128,            7680+ 256,  128);
		API_polar::template f  <  64>(   l, 7936+   0, 7936+  64,            7936+ 128,   64);
		API_polar::template f  <  32>(   l, 8064+   0, 8064+  32,            8064+  64,   32);
		re7(8128, 512);
		API_polar::template gr <  32>(s, l, 8064+   0, 8064+  32,  512+   0, 8064+  64,   32);
		API_polar::template f  <  16>(   l, 8128+   0, 8128+  16,            8128+  32,   16);
		re8(8160, 544);
		API_polar::template gr <  16>(s, l, 8128+   0, 8128+  16,  544+   0, 8128+  32,   16);
		re9r011r111r110(8160, 560);
		API_polar::template xo <  16>(s,     544+   0,  544+  16,             544+   0,   16);
		API_polar::template xo <  32>(s,     512+   0,  512+  32,             512+   0,   32);
		API_polar::template g  <  64>(s, l, 7936+   0, 7936+  64,  512+   0, 7936+ 128,   64);
		API_polar::template f  <  32>(   l, 8064+   0, 8064+  32,            8064+  64,   32);
		API_polar::template f  <  16>(   l, 8128+   0, 8128+  16,            8128+  32,   16);
		re8(8160, 576);
		API_polar::template gr <  16>(s, l, 8128+   0, 8128+  16,  576+   0, 8128+  32,   16);
		r010s10s9(8160, 592);
		API_polar::template xo <  16>(s,     576+   0,  576+  16,             576+   0,   16);
		API_polar::template g  <  32>(s, l, 8064+   0, 8064+  32,  576+   0, 8064+  64,   32);
		re10s10s9s8(8128, 608);
		API_polar::template xo <  32>(s,     576+   0,  576+  32,             576+   0,   32);
		API_polar::template xo <  64>(s,     512+   0,  512+  64,             512+   0,   64);
		API_polar::template g  < 128>(s, l, 7680+   0, 7680+ 128,  512+   0, 7680+ 256,  128);
		API_polar::template f  <  64>(   l, 7936+   0, 7936+  64,            7936+ 128,   64);
		API_polar::template f  <  32>(   l, 8064+   0, 8064+  32,            8064+  64,   32);
		API_polar::template f  <  16>(   l, 8128+   0, 8128+  16,            8128+  32,   16);
		r09re10s10(8160, 640);
		API_polar::template g  <  16>(s, l, 8128+   0, 8128+  16,  640+   0, 8128+  32,   16);
		re10s10s9(8160, 656);
		API_polar::template xo <  16>(s,     640+   0,  640+  16,             640+   0,   16);
		API_polar::template g  <  32>(s, l, 8064+   0, 8064+  32,  640+   0, 8064+  64,   32);
		re10s10s9s8(8128, 672);
		API_polar::template xo <  32>(s,     640+   0,  640+  32,             640+   0,   32);
		API_polar::template g  <  64>(s, l, 7936+   0, 7936+  64,  640+   0, 7936+ 128,   64);
		API_polar::template f  <  32>(   l, 8064+   0, 8064+  32,            8064+  64,   32);
		re10s10r19r18(8128, 704);
		API_polar::template g  <  32>(s, l, 8064+   0, 8064+  32,  704+   0, 8064+  64,   32);
		r17(8128, 736);
		API_polar::template xo <  32>(s,     704+   0,  704+  32,             704+   0,   32);
		API_polar::template xo <  64>(s,     640+   0,  640+  64,             640+   0,   64);
		API_polar::template xo < 128>(s,     512+   0,  512+ 128,             512+   0,  128);
		API_polar::template g  < 256>(s, l, 7168+   0, 7168+ 256,  512+   0, 7168+ 512,  256);
		API_polar::template f  < 128>(   l, 7680+   0, 7680+ 128,            7680+ 256,  128);
		API_polar::template f  <  64>(   l, 7936+   0, 7936+  64,            7936+ 128,   64);
		API_polar::template f  <  32>(   l, 8064+   0, 8064+  32,            8064+  64,   32);
		re9re10s10re10s10s9(8128, 768);
		API_polar::template g  <  32>(s, l, 8064+   0, 8064+  32,  768+   0, 8064+  64,   32);
		API_polar::template f  <  16>(   l, 8128+   0, 8128+  16,            8128+  32,   16);
		re10r110r19(8160, 800);
		API_polar::template g  <  16>(s, l, 8128+   0, 8128+  16,  800+   0, 8128+  32,   16);
		r18(8160, 816);
		API_polar::template xo <  16>(s,     800+   0,  800+  16,             800+   0,   16);
		API_polar::template xo <  32>(s,     768+   0,  768+  32,             768+   0,   32);
		API_polar::template g  <  64>(s, l, 7936+   0, 7936+  64,  768+   0, 7936+ 128,   64);
		s6(8064, 832);
		API_polar::template xo <  64>(s,     768+   0,  768+  64,             768+   0,   64);
		API_polar::template g  < 128>(s, l, 7680+   0, 7680+ 128,  768+   0, 7680+ 256,  128);
		s5(7936, 896);
		API_polar::template xo < 128>(s,     768+   0,  768+ 128,             768+   0,  128);
		API_polar::template xo < 256>(s,     512+   0,  512+ 256,             512+   0,  256);
		API_polar::template xo < 512>(s,       0+   0,    0+ 512,               0+   0,  512);
		API_polar::template g  <1024>(s, l, 4096+   0, 4096+1024,    0+   0, 4096+2048, 1024);
		API_polar::template f  < 512>(   l, 6144+   0, 6144+ 512,            6144+1024,  512);
		API_polar::template f  < 256>(   l, 7168+   0, 7168+ 256,            7168+ 512,  256);
		API_polar::template f  < 128>(   l, 7680+   0, 7680+ 128,            7680+ 256,  128);
		API_polar::template f  <  64>(   l, 7936+   0, 7936+  64,            7936+ 128,   64);
		API_polar::template f  <  32>(   l, 8064+   0, 8064+  32,            8064+  64,   32);
		re7(8128, 1024);
		API_polar::template gr <  32>(s, l, 8064+   0, 8064+  32, 1024+   0, 8064+  64,   32);
		API_polar::template f  <  16>(   l, 8128+   0, 8128+  16,            8128+  32,   16);
		r09(8176, 1056);
		API_polar::template g0 <   8>(   l, 8160+   0, 8160+   8,            8160+  16,    8);
		r010s10(8176, 1064);
		API_polar::template xo0<   8>(s,    1056+   8,                       1056+   0,    8);
		API_polar::template g  <  16>(s, l, 8128+   0, 8128+  16, 1056+   0, 8128+  32,   16);
		re10s10s9(8160, 1072);
		API_polar::template xo <  16>(s,    1056+   0, 1056+  16,            1056+   0,   16);
		API_polar::template xo <  32>(s,    1024+   0, 1024+  32,            1024+   0,   32);
		API_polar::template g  <  64>(s, l, 7936+   0, 7936+  64, 1024+   0, 7936+ 128,   64);
		API_polar::template f  <  32>(   l, 8064+   0, 8064+  32,            8064+  64,   32);
		re9re10s10re10s10s9(8128, 1088);
		API_polar::template g  <  32>(s, l, 8064+   0, 8064+  32, 1088+   0, 8064+  64,   32);
		re10s10r19r18(8128, 1120);
		API_polar::template xo <  32>(s,    1088+   0, 1088+  32,            1088+   0,   32);
		API_polar::template xo <  64>(s,    1024+   0, 1024+  64,            1024+   0,   64);
		API_polar::template g  < 128>(s, l, 7680+   0, 7680+ 128, 1024+   0, 7680+ 256,  128);
		API_polar::template f  <  64>(   l, 7936+   0, 7936+  64,            7936+ 128,   64);
		API_polar::template f  <  32>(   l, 8064+   0, 8064+  32,            8064+  64,   32);
		re9re10s10re10r110r19(8128, 1152);
		API_polar::template g  <  32>(s, l, 8064+   0, 8064+  32, 1152+   0, 8064+  64,   32);
		s7(8128, 1184);
		API_polar::template xo <  32>(s,    1152+   0, 1152+  32,            1152+   0,   32);
		API_polar::template g  <  64>(s, l, 7936+   0, 7936+  64, 1152+   0, 7936+ 128,   64);
		s6(8064, 1216);
		API_polar::template xo <  64>(s,    1152+   0, 1152+  64,            1152+   0,   64);
		API_polar::template xo < 128>(s,    1024+   0, 1024+ 128,            1024+   0,  128);
		API_polar::template g  < 256>(s, l, 7168+   0, 7168+ 256, 1024+   0, 7168+ 512,  256);
		API_polar::template f  < 128>(   l, 7680+   0, 7680+ 128,            7680+ 256,  128);
		API_polar::template f  <  64>(   l, 7936+   0, 7936+  64,            7936+ 128,   64);
		re9r011r111r110s8s7(8064, 1280);
		API_polar::template g  <  64>(s, l, 7936+   0, 7936+  64, 1280+   0, 7936+ 128,   64);
		s6(8064, 1344);
		API_polar::template xo <  64>(s,    1280+   0, 1280+  64,            1280+   0,   64);
		API_polar::template g  < 128>(s, l, 7680+   0, 7680+ 128, 1280+   0, 7680+ 256,  128);
		s5(7936, 1408);
		API_polar::template xo < 128>(s,    1280+   0, 1280+ 128,            1280+   0,  128);
		API_polar::template xo < 256>(s,    1024+   0, 1024+ 256,            1024+   0,  256);
		API_polar::template g  < 512>(s, l, 6144+   0, 6144+ 512, 1024+   0, 6144+1024,  512);
		API_polar::template f  < 256>(   l, 7168+   0, 7168+ 256,            7168+ 512,  256);
		API_polar::template f  < 128>(   l, 7680+   0, 7680+ 128,            7680+ 256,  128);
		r010s10s9s8s7s6(7936, 1536);
		API_polar::template g  < 128>(s, l, 7680+   0, 7680+ 128, 1536+   0, 7680+ 256,  128);
		s5(7936, 1664);
		API_polar::template xo < 128>(s,    1536+   0, 1536+ 128,            1536+   0,  128);
		API_polar::template g  < 256>(s, l, 7168+   0, 7168+ 256, 1536+   0, 7168+ 512,  256);
		r14(7680, 1792);
		API_polar::template xo < 256>(s,    1536+   0, 1536+ 256,            1536+   0,  256);
		API_polar::template xo < 512>(s,    1024+   0, 1024+ 512,            1024+   0,  512);
		API_polar::template xo <1024>(s,       0+   0,    0+1024,               0+   0, 1024);
		API_polar::template g  <2048>(s, l,    0+   0,    0+2048,    0+   0,    0+4096, 2048);
		API_polar::template f  <1024>(   l, 4096+   0, 4096+1024,            4096+2048, 1024);
		API_polar::template f  < 512>(   l, 6144+   0, 6144+ 512,            6144+1024,  512);
		API_polar::template f  < 256>(   l, 7168+   0, 7168+ 256,            7168+ 512,  256);
		API_polar::template f  < 128>(   l, 7680+   0, 7680+ 128,            7680+ 256,  128);
		API_polar::template f  <  64>(   l, 7936+   0, 7936+  64,            7936+ 128,   64);
		API_polar::template f  <  32>(   l, 8064+   0, 8064+  32,            8064+  64,   32);
		r08re9re10s10(8128, 2048);
		API_polar::template g  <  32>(s, l, 8064+   0, 8064+  32, 2048+   0, 8064+  64,   32);
		re9re10s10re10r110r19(8128, 2080);
		API_polar::template xo <  32>(s,    2048+   0, 2048+  32,            2048+   0,   32);
		API_polar::template g  <  64>(s, l, 7936+   0, 7936+  64, 2048+   0, 7936+ 128,   64);
		re9r011r111r110s8s7(8064, 2112);
		API_polar::template xo <  64>(s,    2048+   0, 2048+  64,            2048+   0,   64);
		API_polar::template g  < 128>(s, l, 7680+   0, 7680+ 128, 2048+   0, 7680+ 256,  128);
		r010s10s9s8s7s6(7936, 2176);
		API_polar::template xo < 128>(s,    2048+   0, 2048+ 128,            2048+   0,  128);
		API_polar::template g  < 256>(s, l, 7168+   0, 7168+ 256, 2048+   0, 7168+ 512,  256);
		API_polar::template f  < 128>(   l, 7680+   0, 7680+ 128,            7680+ 256,  128);
		API_polar::template f  <  64>(   l, 7936+   0, 7936+  64,            7936+ 128,   64);
		re10s10s9s8s7(8064, 2304);
		API_polar::template g  <  64>(s, l, 7936+   0, 7936+  64, 2304+   0, 7936+ 128,   64);
		s6(8064, 2368);
		API_polar::template xo <  64>(s,    2304+   0, 2304+  64,            2304+   0,   64);
		API_polar::template g  < 128>(s, l, 7680+   0, 7680+ 128, 2304+   0, 7680+ 256,  128);
		r15(7936, 2432);
		API_polar::template xo < 128>(s,    2304+   0, 2304+ 128,            2304+   0,  128);
		API_polar::template xo < 256>(s,    2048+   0, 2048+ 256,            2048+   0,  256);
		API_polar::template g  < 512>(s, l, 6144+   0, 6144+ 512, 2048+   0, 6144+1024,  512);
		API_polar::template f  < 256>(   l, 7168+   0, 7168+ 256,            7168+ 512,  256);
		API_polar::template f  < 128>(   l, 7680+   0, 7680+ 128,            7680+ 256,  128);
		API_polar::template f  <  64>(   l, 7936+   0, 7936+  64,            7936+ 128,   64);
		re10s10s9s8s7(8064, 2560);
		API_polar::template g  <  64>(s, l, 7936+   0, 7936+  64, 2560+   0, 7936+ 128,   64);
		r16(8064, 2624);
		API_polar::template xo <  64>(s,    2560+   0, 2560+  64,            2560+   0,   64);
		API_polar::template g  < 128>(s, l, 7680+   0, 7680+ 128, 2560+   0, 7680+ 256,  128);
		r15(7936, 2688);
		API_polar::template xo < 128>(s,    2560+   0, 2560+ 128,            2560+   0,  128);
		API_polar::template g  < 256>(s, l, 7168+   0, 7168+ 256, 2560+   0, 7168+ 512,  256);
		r14(7680, 2816);
		API_polar::template xo < 256>(s,    2560+   0, 2560+ 256,            2560+   0,  256);
		API_polar::template xo < 512>(s,    2048+   0, 2048+ 512,            2048+   0,  512);
		API_polar::template g  <1024>(s, l, 4096+   0, 4096+1024, 2048+   0, 4096+2048, 1024);
		API_polar::template f  < 512>(   l, 6144+   0, 6144+ 512,            6144+1024,  512);
		API_polar::template f  < 256>(   l, 7168+   0, 7168+ 256,            7168+ 512,  256);
		API_polar::template f  < 128>(   l, 7680+   0, 7680+ 128,            7680+ 256,  128);
		API_polar::template f  <  64>(   l, 7936+   0, 7936+  64,            7936+ 128,   64);
		API_polar::template f  <  32>(   l, 8064+   0, 8064+  32,            8064+  64,   32);
		API_polar::template f  <  16>(   l, 8128+   0, 8128+  16,            8128+  32,   16);
		re10s10s9(8160, 3072);
		API_polar::template g  <  16>(s, l, 8128+   0, 8128+  16, 3072+   0, 8128+  32,   16);
		r18(8160, 3088);
		API_polar::template xo <  16>(s,    3072+   0, 3072+  16,            3072+   0,   16);
		API_polar::template g  <  32>(s, l, 8064+   0, 8064+  32, 3072+   0, 8064+  64,   32);
		r17(8128, 3104);
		API_polar::template xo <  32>(s,    3072+   0, 3072+  32,            3072+   0,   32);
		API_polar::template g  <  64>(s, l, 7936+   0, 7936+  64, 3072+   0, 7936+ 128,   64);
		r16(8064, 3136);
		API_polar::template xo <  64>(s,    3072+   0, 3072+  64,            3072+   0,   64);
		API_polar::template g  < 128>(s, l, 7680+   0, 7680+ 128, 3072+   0, 7680+ 256,  128);
		r15(7936, 3200);
		API_polar::template xo < 128>(s,    3072+   0, 3072+ 128,            3072+   0,  128);
		API_polar::template g  < 256>(s, l, 7168+   0, 7168+ 256, 3072+   0, 7168+ 512,  256);
		r14(7680, 3328);
		API_polar::template xo < 256>(s,    3072+   0, 3072+ 256,            3072+   0,  256);
		API_polar::template g  < 512>(s, l, 6144+   0, 6144+ 512, 3072+   0, 6144+1024,  512);
		API_polar::template h  < 512>(s, l, 7168+   0,                       3584+   0,  512);
		API_polar::template xo < 512>(s,    3072+   0, 3072+ 512,            3072+   0,  512);
		API_polar::template xo <1024>(s,    2048+   0, 2048+1024,            2048+   0, 1024);
		API_polar::template xo <2048>(s,       0+   0,    0+2048,               0+   0, 2048);
	}
};
}
}
#endif
