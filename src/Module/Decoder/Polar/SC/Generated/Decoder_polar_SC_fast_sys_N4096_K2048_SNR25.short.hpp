#ifndef DECODER_POLAR_SC_FAST_SYS_N4096_K2048_SNR25_HPP_
#define DECODER_POLAR_SC_FAST_SYS_N4096_K2048_SNR25_HPP_

#include <cassert>

#include "../Decoder_polar_SC_fast_sys.hpp"

namespace aff3ct
{
namespace module
{
static const char Frozen_bits_4096_2048_25[4096] = {
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

template <typename B, typename R, class API_polar>
class Decoder_polar_SC_fast_sys_N4096_K2048_SNR25 : public Decoder_polar_SC_fast_sys<B, R, API_polar>
{
public:
	Decoder_polar_SC_fast_sys_N4096_K2048_SNR25(const int& K, const int& N, const mipp::vector<B>& frozen_bits, const int n_frames = 1)
	: Decoder_polar_SC_fast_sys<B, R, API_polar>(K, N, frozen_bits, n_frames)
	{
		assert(N == 4096);
		assert(K == 2048);
		
		auto i = 0;
		while (i < 4096 && Frozen_bits_4096_2048_25[i] == frozen_bits[i]) i++;
		assert(i == 4096);
	}

	virtual ~Decoder_polar_SC_fast_sys_N4096_K2048_SNR25()
	{
	}

	// depth = 5, reverse depth = 7, size = 128, calls = 3
	inline void r05(const int off_l, const int off_s)
	{
	}

	// depth = 6, reverse depth = 6, size = 64, calls = 2
	inline void r06(const int off_l, const int off_s)
	{
	}

	// depth = 7, reverse depth = 5, size = 32, calls = 6
	inline void r07(const int off_l, const int off_s)
	{
	}

	// depth = 8, reverse depth = 4, size = 16, calls = 7
	inline void r08(const int off_l, const int off_s)
	{
	}

	// depth = 9, reverse depth = 3, size = 8, calls = 11
	__attribute__((always_inline))
	inline void r09(const int off_l, const int off_s)
	{
	}

	// depth = 10, reverse depth = 2, size = 4, calls = 44
	__attribute__((always_inline))
	inline void re10(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template rep<   4>(s, l, off_l+   0,                       off_s+   0,    4);
	}

	// depth = 10, reverse depth = 2, size = 4, calls = 47
	__attribute__((always_inline))
	inline void s10(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template spc<   4>(s, l, off_l+   0,                       off_s+   0,    4);
	}

	// depth = 9, reverse depth = 3, size = 8, calls = 41
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

	// depth = 8, reverse depth = 4, size = 16, calls = 7
	inline void r09re10s10(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		r09(off_l+16, off_s+0);
		API_polar::template g0 <   8>(   l, off_l+   0, off_l+   8,            off_l+  16,    8);
		re10s10(off_l+16, off_s+8);
		API_polar::template xo0<   8>(s,    off_s+   8,                       off_s+   0,    8);
	}

	// depth = 7, reverse depth = 5, size = 32, calls = 4
	inline void r08r09re10s10(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		r08(off_l+32, off_s+0);
		API_polar::template g0 <  16>(   l, off_l+   0, off_l+  16,            off_l+  32,   16);
		r09re10s10(off_l+32, off_s+16);
		API_polar::template xo0<  16>(s,    off_s+  16,                       off_s+   0,   16);
	}

	// depth = 6, reverse depth = 6, size = 64, calls = 2
	inline void r07r08r09re10s10(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		r07(off_l+64, off_s+0);
		API_polar::template g0 <  32>(   l, off_l+   0, off_l+  32,            off_l+  64,   32);
		r08r09re10s10(off_l+64, off_s+32);
		API_polar::template xo0<  32>(s,    off_s+  32,                       off_s+   0,   32);
	}

	// depth = 8, reverse depth = 4, size = 16, calls = 9
	inline void re8(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template rep<  16>(s, l, off_l+   0,                       off_s+   0,   16);
	}

	// depth = 9, reverse depth = 3, size = 8, calls = 17
	__attribute__((always_inline))
	inline void re9(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template rep<   8>(s, l, off_l+   0,                       off_s+   0,    8);
	}

	// depth = 8, reverse depth = 4, size = 16, calls = 13
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

	// depth = 7, reverse depth = 5, size = 32, calls = 5
	inline void re8re9re10s10(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <  16>(   l, off_l+   0, off_l+  16,            off_l+  32,   16);
		re8(off_l+32, off_s+0);
		API_polar::template gr <  16>(s, l, off_l+   0, off_l+  16, off_s+   0, off_l+  32,   16);
		re9re10s10(off_l+32, off_s+16);
		API_polar::template xo <  16>(s,    off_s+   0, off_s+  16,            off_s+   0,   16);
	}

	// depth = 6, reverse depth = 6, size = 64, calls = 2
	inline void r07re8re9re10s10(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		r07(off_l+64, off_s+0);
		API_polar::template g0 <  32>(   l, off_l+   0, off_l+  32,            off_l+  64,   32);
		re8re9re10s10(off_l+64, off_s+32);
		API_polar::template xo0<  32>(s,    off_s+  32,                       off_s+   0,   32);
	}

	// depth = 6, reverse depth = 6, size = 64, calls = 2
	inline void re6(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template rep<  64>(s, l, off_l+   0,                       off_s+   0,   64);
	}

	// depth = 7, reverse depth = 5, size = 32, calls = 3
	inline void re7(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template rep<  32>(s, l, off_l+   0,                       off_s+   0,   32);
	}

	// depth = 10, reverse depth = 2, size = 4, calls = 10
	__attribute__((always_inline))
	inline void r010(const int off_l, const int off_s)
	{
	}

	// depth = 9, reverse depth = 3, size = 8, calls = 6
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

	// depth = 9, reverse depth = 3, size = 8, calls = 25
	__attribute__((always_inline))
	inline void s9(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template spc<   8>(s, l, off_l+   0,                       off_s+   0,    8);
	}

	// depth = 8, reverse depth = 4, size = 16, calls = 4
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

	// depth = 7, reverse depth = 5, size = 32, calls = 2
	inline void re8r010s10s9(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <  16>(   l, off_l+   0, off_l+  16,            off_l+  32,   16);
		re8(off_l+32, off_s+0);
		API_polar::template gr <  16>(s, l, off_l+   0, off_l+  16, off_s+   0, off_l+  32,   16);
		r010s10s9(off_l+32, off_s+16);
		API_polar::template xo <  16>(s,    off_s+   0, off_s+  16,            off_s+   0,   16);
	}

	// depth = 6, reverse depth = 6, size = 64, calls = 2
	inline void re7re8r010s10s9(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <  32>(   l, off_l+   0, off_l+  32,            off_l+  64,   32);
		re7(off_l+64, off_s+0);
		API_polar::template gr <  32>(s, l, off_l+   0, off_l+  32, off_s+   0, off_l+  64,   32);
		re8r010s10s9(off_l+64, off_s+32);
		API_polar::template xo <  32>(s,    off_s+   0, off_s+  32,            off_s+   0,   32);
	}

	// depth = 11, reverse depth = 1, size = 2, calls = 8
	__attribute__((always_inline))
	inline void r011(const int off_l, const int off_s)
	{
	}

	// depth = 11, reverse depth = 1, size = 2, calls = 8
	__attribute__((always_inline))
	inline void r111(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template h  <   2>(s, l, off_l+   0,                       off_s+   0,    2);
	}

	// depth = 10, reverse depth = 2, size = 4, calls = 8
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

	// depth = 9, reverse depth = 3, size = 8, calls = 4
	__attribute__((always_inline))
	inline void r010r011r111(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		r010(off_l+8, off_s+0);
		API_polar::template g0 <   4>(   l, off_l+   0, off_l+   4,            off_l+   8,    4);
		r011r111(off_l+8, off_s+4);
		API_polar::template xo0<   4>(s,    off_s+   4,                       off_s+   0,    4);
	}

	// depth = 8, reverse depth = 4, size = 16, calls = 2
	inline void r09r010r011r111(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		r09(off_l+16, off_s+0);
		API_polar::template g0 <   8>(   l, off_l+   0, off_l+   8,            off_l+  16,    8);
		r010r011r111(off_l+16, off_s+8);
		API_polar::template xo0<   8>(s,    off_s+   8,                       off_s+   0,    8);
	}

	// depth = 7, reverse depth = 5, size = 32, calls = 2
	inline void r08r09r010r011r111(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		r08(off_l+32, off_s+0);
		API_polar::template g0 <  16>(   l, off_l+   0, off_l+  16,            off_l+  32,   16);
		r09r010r011r111(off_l+32, off_s+16);
		API_polar::template xo0<  16>(s,    off_s+  16,                       off_s+   0,   16);
	}

	// depth = 8, reverse depth = 4, size = 16, calls = 17
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
	inline void r09re10s10re10s10s9(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <  16>(   l, off_l+   0, off_l+  16,            off_l+  32,   16);
		r09re10s10(off_l+32, off_s+0);
		API_polar::template g  <  16>(s, l, off_l+   0, off_l+  16, off_s+   0, off_l+  32,   16);
		re10s10s9(off_l+32, off_s+16);
		API_polar::template xo <  16>(s,    off_s+   0, off_s+  16,            off_s+   0,   16);
	}

	// depth = 7, reverse depth = 5, size = 32, calls = 4
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

	// depth = 9, reverse depth = 3, size = 8, calls = 9
	__attribute__((always_inline))
	inline void r19(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template h  <   8>(s, l, off_l+   0,                       off_s+   0,    8);
	}

	// depth = 8, reverse depth = 4, size = 16, calls = 4
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

	// depth = 8, reverse depth = 4, size = 16, calls = 5
	inline void r18(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template h  <  16>(s, l, off_l+   0,                       off_s+   0,   16);
	}

	// depth = 7, reverse depth = 5, size = 32, calls = 3
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

	// depth = 6, reverse depth = 6, size = 64, calls = 2
	inline void re9re10s10re10s10s9re10s10r19r18(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <  32>(   l, off_l+   0, off_l+  32,            off_l+  64,   32);
		re9re10s10re10s10s9(off_l+64, off_s+0);
		API_polar::template g  <  32>(s, l, off_l+   0, off_l+  32, off_s+   0, off_l+  64,   32);
		re10s10r19r18(off_l+64, off_s+32);
		API_polar::template xo <  32>(s,    off_s+   0, off_s+  32,            off_s+   0,   32);
	}

	// depth = 8, reverse depth = 4, size = 16, calls = 2
	inline void r09r010s10(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		r09(off_l+16, off_s+0);
		API_polar::template g0 <   8>(   l, off_l+   0, off_l+   8,            off_l+  16,    8);
		r010s10(off_l+16, off_s+8);
		API_polar::template xo0<   8>(s,    off_s+   8,                       off_s+   0,    8);
	}

	// depth = 7, reverse depth = 5, size = 32, calls = 2
	inline void r09r010s10re10s10s9(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <  16>(   l, off_l+   0, off_l+  16,            off_l+  32,   16);
		r09r010s10(off_l+32, off_s+0);
		API_polar::template g  <  16>(s, l, off_l+   0, off_l+  16, off_s+   0, off_l+  32,   16);
		re10s10s9(off_l+32, off_s+16);
		API_polar::template xo <  16>(s,    off_s+   0, off_s+  16,            off_s+   0,   16);
	}

	// depth = 6, reverse depth = 6, size = 64, calls = 2
	inline void r08r09re10s10re9re10s10re10s10s9(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <  32>(   l, off_l+   0, off_l+  32,            off_l+  64,   32);
		r08r09re10s10(off_l+64, off_s+0);
		API_polar::template g  <  32>(s, l, off_l+   0, off_l+  32, off_s+   0, off_l+  64,   32);
		re9re10s10re10s10s9(off_l+64, off_s+32);
		API_polar::template xo <  32>(s,    off_s+   0, off_s+  32,            off_s+   0,   32);
	}

	// depth = 10, reverse depth = 2, size = 4, calls = 7
	__attribute__((always_inline))
	inline void r110(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template h  <   4>(s, l, off_l+   0,                       off_s+   0,    4);
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

	// depth = 8, reverse depth = 4, size = 16, calls = 2
	inline void re9re10r110(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <   8>(   l, off_l+   0, off_l+   8,            off_l+  16,    8);
		re9(off_l+16, off_s+0);
		API_polar::template gr <   8>(s, l, off_l+   0, off_l+   8, off_s+   0, off_l+  16,    8);
		re10r110(off_l+16, off_s+8);
		API_polar::template xo <   8>(s,    off_s+   0, off_s+   8,            off_s+   0,    8);
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
	inline void r011r111r110r19(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <   8>(   l, off_l+   0, off_l+   8,            off_l+  16,    8);
		r011r111r110(off_l+16, off_s+0);
		API_polar::template g  <   8>(s, l, off_l+   0, off_l+   8, off_s+   0, off_l+  16,    8);
		r19(off_l+16, off_s+8);
		API_polar::template xo <   8>(s,    off_s+   0, off_s+   8,            off_s+   0,    8);
	}

	// depth = 8, reverse depth = 4, size = 16, calls = 2
	inline void r010r011r111s9(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <   8>(   l, off_l+   0, off_l+   8,            off_l+  16,    8);
		r010r011r111(off_l+16, off_s+0);
		API_polar::template g  <   8>(s, l, off_l+   0, off_l+   8, off_s+   0, off_l+  16,    8);
		s9(off_l+16, off_s+8);
		API_polar::template xo <   8>(s,    off_s+   0, off_s+   8,            off_s+   0,    8);
	}

	// depth = 8, reverse depth = 4, size = 16, calls = 13
	inline void s8(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template spc<  16>(s, l, off_l+   0,                       off_s+   0,   16);
	}

	// depth = 7, reverse depth = 5, size = 32, calls = 7
	inline void s7(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template spc<  32>(s, l, off_l+   0,                       off_s+   0,   32);
	}

	// depth = 8, reverse depth = 4, size = 16, calls = 2
	inline void re9s9(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <   8>(   l, off_l+   0, off_l+   8,            off_l+  16,    8);
		re9(off_l+16, off_s+0);
		API_polar::template gr <   8>(s, l, off_l+   0, off_l+   8, off_s+   0, off_l+  16,    8);
		s9(off_l+16, off_s+8);
		API_polar::template xo <   8>(s,    off_s+   0, off_s+   8,            off_s+   0,    8);
	}

	// depth = 7, reverse depth = 5, size = 32, calls = 8
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

	// depth = 6, reverse depth = 6, size = 64, calls = 2
	inline void r09re10s10re10s10s9re10s10s9s8(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <  32>(   l, off_l+   0, off_l+  32,            off_l+  64,   32);
		r09re10s10re10s10s9(off_l+64, off_s+0);
		API_polar::template g  <  32>(s, l, off_l+   0, off_l+  32, off_s+   0, off_l+  64,   32);
		re10s10s9s8(off_l+64, off_s+32);
		API_polar::template xo <  32>(s,    off_s+   0, off_s+  32,            off_s+   0,   32);
	}

	// depth = 7, reverse depth = 5, size = 32, calls = 3
	inline void r17(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template h  <  32>(s, l, off_l+   0,                       off_s+   0,   32);
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

	// depth = 7, reverse depth = 5, size = 32, calls = 2
	inline void r011r111r110r19r18(const int off_l, const int off_s)
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <  16>(   l, off_l+   0, off_l+  16,            off_l+  32,   16);
		r011r111r110r19(off_l+32, off_s+0);
		API_polar::template g  <  16>(s, l, off_l+   0, off_l+  16, off_s+   0, off_l+  32,   16);
		r18(off_l+32, off_s+16);
		API_polar::template xo <  16>(s,    off_s+   0, off_s+  16,            off_s+   0,   16);
	}

	// depth = 6, reverse depth = 6, size = 64, calls = 4
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

	void decode()
	{
		auto &l = this->l;
		auto &s = this->s;

		API_polar::template f  <2048>(   l,    0+   0,    0+2048,               0+4096, 2048);
		API_polar::template f  <1024>(   l, 4096+   0, 4096+1024,            4096+2048, 1024);
		API_polar::template f  < 512>(   l, 6144+   0, 6144+ 512,            6144+1024,  512);
		API_polar::template g0 < 256>(   l, 7168+   0, 7168+ 256,            7168+ 512,  256);
		r05(7936, 256);
		API_polar::template g0 < 128>(   l, 7680+   0, 7680+ 128,            7680+ 256,  128);
		r06(8064, 384);
		API_polar::template g0 <  64>(   l, 7936+   0, 7936+  64,            7936+ 128,   64);
		r07r08r09re10s10(8064, 448);
		API_polar::template xo0<  64>(s,     384+  64,                        384+   0,   64);
		API_polar::template xo0< 128>(s,     256+ 128,                        256+   0,  128);
		API_polar::template xo0< 256>(s,       0+ 256,                          0+   0,  256);
		API_polar::template g  < 512>(s, l, 6144+   0, 6144+ 512,    0+   0, 6144+1024,  512);
		API_polar::template f  < 256>(   l, 7168+   0, 7168+ 256,            7168+ 512,  256);
		r05(7936, 512);
		API_polar::template g0 < 128>(   l, 7680+   0, 7680+ 128,            7680+ 256,  128);
		r06(8064, 640);
		API_polar::template g0 <  64>(   l, 7936+   0, 7936+  64,            7936+ 128,   64);
		r07re8re9re10s10(8064, 704);
		API_polar::template xo0<  64>(s,     640+  64,                        640+   0,   64);
		API_polar::template xo0< 128>(s,     512+ 128,                        512+   0,  128);
		API_polar::template g  < 256>(s, l, 7168+   0, 7168+ 256,  512+   0, 7168+ 512,  256);
		API_polar::template f  < 128>(   l, 7680+   0, 7680+ 128,            7680+ 256,  128);
		API_polar::template f  <  64>(   l, 7936+   0, 7936+  64,            7936+ 128,   64);
		re6(8064, 768);
		API_polar::template gr <  64>(s, l, 7936+   0, 7936+  64,  768+   0, 7936+ 128,   64);
		re7re8r010s10s9(8064, 832);
		API_polar::template xo <  64>(s,     768+   0,  768+  64,             768+   0,   64);
		API_polar::template g  < 128>(s, l, 7680+   0, 7680+ 128,  768+   0, 7680+ 256,  128);
		API_polar::template f  <  64>(   l, 7936+   0, 7936+  64,            7936+ 128,   64);
		API_polar::template f  <  32>(   l, 8064+   0, 8064+  32,            8064+  64,   32);
		r08r09r010r011r111(8128, 896);
		API_polar::template g  <  32>(s, l, 8064+   0, 8064+  32,  896+   0, 8064+  64,   32);
		r09re10s10re10s10s9(8128, 928);
		API_polar::template xo <  32>(s,     896+   0,  896+  32,             896+   0,   32);
		API_polar::template g  <  64>(s, l, 7936+   0, 7936+  64,  896+   0, 7936+ 128,   64);
		re9re10s10re10s10s9re10s10r19r18(8064, 960);
		API_polar::template xo <  64>(s,     896+   0,  896+  64,             896+   0,   64);
		API_polar::template xo < 128>(s,     768+   0,  768+ 128,             768+   0,  128);
		API_polar::template xo < 256>(s,     512+   0,  512+ 256,             512+   0,  256);
		API_polar::template xo < 512>(s,       0+   0,    0+ 512,               0+   0,  512);
		API_polar::template g  <1024>(s, l, 4096+   0, 4096+1024,    0+   0, 4096+2048, 1024);
		API_polar::template f  < 512>(   l, 6144+   0, 6144+ 512,            6144+1024,  512);
		API_polar::template f  < 256>(   l, 7168+   0, 7168+ 256,            7168+ 512,  256);
		r05(7936, 1024);
		API_polar::template g0 < 128>(   l, 7680+   0, 7680+ 128,            7680+ 256,  128);
		API_polar::template f  <  64>(   l, 7936+   0, 7936+  64,            7936+ 128,   64);
		re6(8064, 1152);
		API_polar::template gr <  64>(s, l, 7936+   0, 7936+  64, 1152+   0, 7936+ 128,   64);
		API_polar::template f  <  32>(   l, 8064+   0, 8064+  32,            8064+  64,   32);
		re7(8128, 1216);
		API_polar::template gr <  32>(s, l, 8064+   0, 8064+  32, 1216+   0, 8064+  64,   32);
		r09r010s10re10s10s9(8128, 1248);
		API_polar::template xo <  32>(s,    1216+   0, 1216+  32,            1216+   0,   32);
		API_polar::template xo <  64>(s,    1152+   0, 1152+  64,            1152+   0,   64);
		API_polar::template xo0< 128>(s,    1024+ 128,                       1024+   0,  128);
		API_polar::template g  < 256>(s, l, 7168+   0, 7168+ 256, 1024+   0, 7168+ 512,  256);
		API_polar::template f  < 128>(   l, 7680+   0, 7680+ 128,            7680+ 256,  128);
		API_polar::template f  <  64>(   l, 7936+   0, 7936+  64,            7936+ 128,   64);
		r07(8128, 1280);
		API_polar::template g0 <  32>(   l, 8064+   0, 8064+  32,            8064+  64,   32);
		r08r09r010r011r111(8128, 1312);
		API_polar::template xo0<  32>(s,    1280+  32,                       1280+   0,   32);
		API_polar::template g  <  64>(s, l, 7936+   0, 7936+  64, 1280+   0, 7936+ 128,   64);
		r08r09re10s10re9re10s10re10s10s9(8064, 1344);
		API_polar::template xo <  64>(s,    1280+   0, 1280+  64,            1280+   0,   64);
		API_polar::template g  < 128>(s, l, 7680+   0, 7680+ 128, 1280+   0, 7680+ 256,  128);
		API_polar::template f  <  64>(   l, 7936+   0, 7936+  64,            7936+ 128,   64);
		API_polar::template f  <  32>(   l, 8064+   0, 8064+  32,            8064+  64,   32);
		re8re9re10s10(8128, 1408);
		API_polar::template g  <  32>(s, l, 8064+   0, 8064+  32, 1408+   0, 8064+  64,   32);
		API_polar::template f  <  16>(   l, 8128+   0, 8128+  16,            8128+  32,   16);
		re9re10r110(8160, 1440);
		API_polar::template g  <  16>(s, l, 8128+   0, 8128+  16, 1440+   0, 8128+  32,   16);
		r011r111r110r19(8160, 1456);
		API_polar::template xo <  16>(s,    1440+   0, 1440+  16,            1440+   0,   16);
		API_polar::template xo <  32>(s,    1408+   0, 1408+  32,            1408+   0,   32);
		API_polar::template g  <  64>(s, l, 7936+   0, 7936+  64, 1408+   0, 7936+ 128,   64);
		API_polar::template f  <  32>(   l, 8064+   0, 8064+  32,            8064+  64,   32);
		API_polar::template f  <  16>(   l, 8128+   0, 8128+  16,            8128+  32,   16);
		r010r011r111s9(8160, 1472);
		API_polar::template g  <  16>(s, l, 8128+   0, 8128+  16, 1472+   0, 8128+  32,   16);
		s8(8160, 1488);
		API_polar::template xo <  16>(s,    1472+   0, 1472+  16,            1472+   0,   16);
		API_polar::template g  <  32>(s, l, 8064+   0, 8064+  32, 1472+   0, 8064+  64,   32);
		s7(8128, 1504);
		API_polar::template xo <  32>(s,    1472+   0, 1472+  32,            1472+   0,   32);
		API_polar::template xo <  64>(s,    1408+   0, 1408+  64,            1408+   0,   64);
		API_polar::template xo < 128>(s,    1280+   0, 1280+ 128,            1280+   0,  128);
		API_polar::template xo < 256>(s,    1024+   0, 1024+ 256,            1024+   0,  256);
		API_polar::template g  < 512>(s, l, 6144+   0, 6144+ 512, 1024+   0, 6144+1024,  512);
		API_polar::template f  < 256>(   l, 7168+   0, 7168+ 256,            7168+ 512,  256);
		API_polar::template f  < 128>(   l, 7680+   0, 7680+ 128,            7680+ 256,  128);
		API_polar::template f  <  64>(   l, 7936+   0, 7936+  64,            7936+ 128,   64);
		r07(8128, 1536);
		API_polar::template g0 <  32>(   l, 8064+   0, 8064+  32,            8064+  64,   32);
		r08(8160, 1568);
		API_polar::template g0 <  16>(   l, 8128+   0, 8128+  16,            8128+  32,   16);
		re9re10s10(8160, 1584);
		API_polar::template xo0<  16>(s,    1568+  16,                       1568+   0,   16);
		API_polar::template xo0<  32>(s,    1536+  32,                       1536+   0,   32);
		API_polar::template g  <  64>(s, l, 7936+   0, 7936+  64, 1536+   0, 7936+ 128,   64);
		API_polar::template f  <  32>(   l, 8064+   0, 8064+  32,            8064+  64,   32);
		re8re9re10s10(8128, 1600);
		API_polar::template g  <  32>(s, l, 8064+   0, 8064+  32, 1600+   0, 8064+  64,   32);
		API_polar::template f  <  16>(   l, 8128+   0, 8128+  16,            8128+  32,   16);
		re9s9(8160, 1632);
		API_polar::template g  <  16>(s, l, 8128+   0, 8128+  16, 1632+   0, 8128+  32,   16);
		s8(8160, 1648);
		API_polar::template xo <  16>(s,    1632+   0, 1632+  16,            1632+   0,   16);
		API_polar::template xo <  32>(s,    1600+   0, 1600+  32,            1600+   0,   32);
		API_polar::template xo <  64>(s,    1536+   0, 1536+  64,            1536+   0,   64);
		API_polar::template g  < 128>(s, l, 7680+   0, 7680+ 128, 1536+   0, 7680+ 256,  128);
		API_polar::template f  <  64>(   l, 7936+   0, 7936+  64,            7936+ 128,   64);
		API_polar::template f  <  32>(   l, 8064+   0, 8064+  32,            8064+  64,   32);
		API_polar::template f  <  16>(   l, 8128+   0, 8128+  16,            8128+  32,   16);
		re8(8160, 1664);
		API_polar::template gr <  16>(s, l, 8128+   0, 8128+  16, 1664+   0, 8128+  32,   16);
		r010r011r111s9(8160, 1680);
		API_polar::template xo <  16>(s,    1664+   0, 1664+  16,            1664+   0,   16);
		API_polar::template g  <  32>(s, l, 8064+   0, 8064+  32, 1664+   0, 8064+  64,   32);
		re10s10s9s8(8128, 1696);
		API_polar::template xo <  32>(s,    1664+   0, 1664+  32,            1664+   0,   32);
		API_polar::template g  <  64>(s, l, 7936+   0, 7936+  64, 1664+   0, 7936+ 128,   64);
		re10s10s9s8s7(8064, 1728);
		API_polar::template xo <  64>(s,    1664+   0, 1664+  64,            1664+   0,   64);
		API_polar::template xo < 128>(s,    1536+   0, 1536+ 128,            1536+   0,  128);
		API_polar::template g  < 256>(s, l, 7168+   0, 7168+ 256, 1536+   0, 7168+ 512,  256);
		API_polar::template f  < 128>(   l, 7680+   0, 7680+ 128,            7680+ 256,  128);
		API_polar::template f  <  64>(   l, 7936+   0, 7936+  64,            7936+ 128,   64);
		r09re10s10re10s10s9re10s10s9s8(8064, 1792);
		API_polar::template g  <  64>(s, l, 7936+   0, 7936+  64, 1792+   0, 7936+ 128,   64);
		API_polar::template f  <  32>(   l, 8064+   0, 8064+  32,            8064+  64,   32);
		re10s10s9s8(8128, 1856);
		API_polar::template g  <  32>(s, l, 8064+   0, 8064+  32, 1856+   0, 8064+  64,   32);
		r17(8128, 1888);
		API_polar::template xo <  32>(s,    1856+   0, 1856+  32,            1856+   0,   32);
		API_polar::template xo <  64>(s,    1792+   0, 1792+  64,            1792+   0,   64);
		API_polar::template g  < 128>(s, l, 7680+   0, 7680+ 128, 1792+   0, 7680+ 256,  128);
		API_polar::template f  <  64>(   l, 7936+   0, 7936+  64,            7936+ 128,   64);
		API_polar::template f  <  32>(   l, 8064+   0, 8064+  32,            8064+  64,   32);
		re10s10r19r18(8128, 1920);
		API_polar::template g  <  32>(s, l, 8064+   0, 8064+  32, 1920+   0, 8064+  64,   32);
		r17(8128, 1952);
		API_polar::template xo <  32>(s,    1920+   0, 1920+  32,            1920+   0,   32);
		API_polar::template g  <  64>(s, l, 7936+   0, 7936+  64, 1920+   0, 7936+ 128,   64);
		API_polar::template h  <  64>(s, l, 8064+   0,                       1984+   0,   64);
		API_polar::template xo <  64>(s,    1920+   0, 1920+  64,            1920+   0,   64);
		API_polar::template xo < 128>(s,    1792+   0, 1792+ 128,            1792+   0,  128);
		API_polar::template xo < 256>(s,    1536+   0, 1536+ 256,            1536+   0,  256);
		API_polar::template xo < 512>(s,    1024+   0, 1024+ 512,            1024+   0,  512);
		API_polar::template xo <1024>(s,       0+   0,    0+1024,               0+   0, 1024);
		API_polar::template g  <2048>(s, l,    0+   0,    0+2048,    0+   0,    0+4096, 2048);
		API_polar::template f  <1024>(   l, 4096+   0, 4096+1024,            4096+2048, 1024);
		API_polar::template f  < 512>(   l, 6144+   0, 6144+ 512,            6144+1024,  512);
		API_polar::template f  < 256>(   l, 7168+   0, 7168+ 256,            7168+ 512,  256);
		API_polar::template f  < 128>(   l, 7680+   0, 7680+ 128,            7680+ 256,  128);
		API_polar::template rep< 128>(s, l, 7936+   0,                       2048+   0,  128);
		API_polar::template gr < 128>(s, l, 7680+   0, 7680+ 128, 2048+   0, 7680+ 256,  128);
		API_polar::template f  <  64>(   l, 7936+   0, 7936+  64,            7936+ 128,   64);
		r07r08r09re10s10(8064, 2176);
		API_polar::template g  <  64>(s, l, 7936+   0, 7936+  64, 2176+   0, 7936+ 128,   64);
		API_polar::template f  <  32>(   l, 8064+   0, 8064+  32,            8064+  64,   32);
		re8re9re10s10(8128, 2240);
		API_polar::template g  <  32>(s, l, 8064+   0, 8064+  32, 2240+   0, 8064+  64,   32);
		API_polar::template f  <  16>(   l, 8128+   0, 8128+  16,            8128+  32,   16);
		re9re10s10(8160, 2272);
		API_polar::template g  <  16>(s, l, 8128+   0, 8128+  16, 2272+   0, 8128+  32,   16);
		r011r111r110r19(8160, 2288);
		API_polar::template xo <  16>(s,    2272+   0, 2272+  16,            2272+   0,   16);
		API_polar::template xo <  32>(s,    2240+   0, 2240+  32,            2240+   0,   32);
		API_polar::template xo <  64>(s,    2176+   0, 2176+  64,            2176+   0,   64);
		API_polar::template xo < 128>(s,    2048+   0, 2048+ 128,            2048+   0,  128);
		API_polar::template g  < 256>(s, l, 7168+   0, 7168+ 256, 2048+   0, 7168+ 512,  256);
		API_polar::template f  < 128>(   l, 7680+   0, 7680+ 128,            7680+ 256,  128);
		API_polar::template f  <  64>(   l, 7936+   0, 7936+  64,            7936+ 128,   64);
		r07re8re9re10s10(8064, 2304);
		API_polar::template g  <  64>(s, l, 7936+   0, 7936+  64, 2304+   0, 7936+ 128,   64);
		API_polar::template f  <  32>(   l, 8064+   0, 8064+  32,            8064+  64,   32);
		API_polar::template f  <  16>(   l, 8128+   0, 8128+  16,            8128+  32,   16);
		re8(8160, 2368);
		API_polar::template gr <  16>(s, l, 8128+   0, 8128+  16, 2368+   0, 8128+  32,   16);
		re9s9(8160, 2384);
		API_polar::template xo <  16>(s,    2368+   0, 2368+  16,            2368+   0,   16);
		API_polar::template g  <  32>(s, l, 8064+   0, 8064+  32, 2368+   0, 8064+  64,   32);
		r010s10s9s8(8128, 2400);
		API_polar::template xo <  32>(s,    2368+   0, 2368+  32,            2368+   0,   32);
		API_polar::template xo <  64>(s,    2304+   0, 2304+  64,            2304+   0,   64);
		API_polar::template g  < 128>(s, l, 7680+   0, 7680+ 128, 2304+   0, 7680+ 256,  128);
		API_polar::template f  <  64>(   l, 7936+   0, 7936+  64,            7936+ 128,   64);
		API_polar::template f  <  32>(   l, 8064+   0, 8064+  32,            8064+  64,   32);
		r09r010s10re10s10s9(8128, 2432);
		API_polar::template g  <  32>(s, l, 8064+   0, 8064+  32, 2432+   0, 8064+  64,   32);
		re10s10s9s8(8128, 2464);
		API_polar::template xo <  32>(s,    2432+   0, 2432+  32,            2432+   0,   32);
		API_polar::template g  <  64>(s, l, 7936+   0, 7936+  64, 2432+   0, 7936+ 128,   64);
		re10s10s9s8s7(8064, 2496);
		API_polar::template xo <  64>(s,    2432+   0, 2432+  64,            2432+   0,   64);
		API_polar::template xo < 128>(s,    2304+   0, 2304+ 128,            2304+   0,  128);
		API_polar::template xo < 256>(s,    2048+   0, 2048+ 256,            2048+   0,  256);
		API_polar::template g  < 512>(s, l, 6144+   0, 6144+ 512, 2048+   0, 6144+1024,  512);
		API_polar::template f  < 256>(   l, 7168+   0, 7168+ 256,            7168+ 512,  256);
		API_polar::template f  < 128>(   l, 7680+   0, 7680+ 128,            7680+ 256,  128);
		API_polar::template f  <  64>(   l, 7936+   0, 7936+  64,            7936+ 128,   64);
		re7re8r010s10s9(8064, 2560);
		API_polar::template g  <  64>(s, l, 7936+   0, 7936+  64, 2560+   0, 7936+ 128,   64);
		r09re10s10re10s10s9re10s10s9s8(8064, 2624);
		API_polar::template xo <  64>(s,    2560+   0, 2560+  64,            2560+   0,   64);
		API_polar::template g  < 128>(s, l, 7680+   0, 7680+ 128, 2560+   0, 7680+ 256,  128);
		API_polar::template f  <  64>(   l, 7936+   0, 7936+  64,            7936+ 128,   64);
		re9re10s10re10s10s9re10s10r19r18(8064, 2688);
		API_polar::template g  <  64>(s, l, 7936+   0, 7936+  64, 2688+   0, 7936+ 128,   64);
		API_polar::template f  <  32>(   l, 8064+   0, 8064+  32,            8064+  64,   32);
		r011r111r110r19r18(8128, 2752);
		API_polar::template g  <  32>(s, l, 8064+   0, 8064+  32, 2752+   0, 8064+  64,   32);
		r17(8128, 2784);
		API_polar::template xo <  32>(s,    2752+   0, 2752+  32,            2752+   0,   32);
		API_polar::template xo <  64>(s,    2688+   0, 2688+  64,            2688+   0,   64);
		API_polar::template xo < 128>(s,    2560+   0, 2560+ 128,            2560+   0,  128);
		API_polar::template g  < 256>(s, l, 7168+   0, 7168+ 256, 2560+   0, 7168+ 512,  256);
		API_polar::template f  < 128>(   l, 7680+   0, 7680+ 128,            7680+ 256,  128);
		API_polar::template f  <  64>(   l, 7936+   0, 7936+  64,            7936+ 128,   64);
		API_polar::template f  <  32>(   l, 8064+   0, 8064+  32,            8064+  64,   32);
		API_polar::template f  <  16>(   l, 8128+   0, 8128+  16,            8128+  32,   16);
		re9re10s10(8160, 2816);
		API_polar::template g  <  16>(s, l, 8128+   0, 8128+  16, 2816+   0, 8128+  32,   16);
		API_polar::template f  <   8>(   l, 8160+   0, 8160+   8,            8160+  16,    8);
		re10r110(8176, 2832);
		API_polar::template g  <   8>(s, l, 8160+   0, 8160+   8, 2832+   0, 8160+  16,    8);
		r19(8176, 2840);
		API_polar::template xo <   8>(s,    2832+   0, 2832+   8,            2832+   0,    8);
		API_polar::template xo <  16>(s,    2816+   0, 2816+  16,            2816+   0,   16);
		API_polar::template g  <  32>(s, l, 8064+   0, 8064+  32, 2816+   0, 8064+  64,   32);
		s7(8128, 2848);
		API_polar::template xo <  32>(s,    2816+   0, 2816+  32,            2816+   0,   32);
		API_polar::template g  <  64>(s, l, 7936+   0, 7936+  64, 2816+   0, 7936+ 128,   64);
		s6(8064, 2880);
		API_polar::template xo <  64>(s,    2816+   0, 2816+  64,            2816+   0,   64);
		API_polar::template g  < 128>(s, l, 7680+   0, 7680+ 128, 2816+   0, 7680+ 256,  128);
		s5(7936, 2944);
		API_polar::template xo < 128>(s,    2816+   0, 2816+ 128,            2816+   0,  128);
		API_polar::template xo < 256>(s,    2560+   0, 2560+ 256,            2560+   0,  256);
		API_polar::template xo < 512>(s,    2048+   0, 2048+ 512,            2048+   0,  512);
		API_polar::template g  <1024>(s, l, 4096+   0, 4096+1024, 2048+   0, 4096+2048, 1024);
		API_polar::template f  < 512>(   l, 6144+   0, 6144+ 512,            6144+1024,  512);
		API_polar::template f  < 256>(   l, 7168+   0, 7168+ 256,            7168+ 512,  256);
		API_polar::template f  < 128>(   l, 7680+   0, 7680+ 128,            7680+ 256,  128);
		API_polar::template f  <  64>(   l, 7936+   0, 7936+  64,            7936+ 128,   64);
		r08r09re10s10re9re10s10re10s10s9(8064, 3072);
		API_polar::template g  <  64>(s, l, 7936+   0, 7936+  64, 3072+   0, 7936+ 128,   64);
		API_polar::template f  <  32>(   l, 8064+   0, 8064+  32,            8064+  64,   32);
		API_polar::template f  <  16>(   l, 8128+   0, 8128+  16,            8128+  32,   16);
		re9re10s10(8160, 3136);
		API_polar::template g  <  16>(s, l, 8128+   0, 8128+  16, 3136+   0, 8128+  32,   16);
		re10s10r19(8160, 3152);
		API_polar::template xo <  16>(s,    3136+   0, 3136+  16,            3136+   0,   16);
		API_polar::template g  <  32>(s, l, 8064+   0, 8064+  32, 3136+   0, 8064+  64,   32);
		r011r111r110r19r18(8128, 3168);
		API_polar::template xo <  32>(s,    3136+   0, 3136+  32,            3136+   0,   32);
		API_polar::template xo <  64>(s,    3072+   0, 3072+  64,            3072+   0,   64);
		API_polar::template g  < 128>(s, l, 7680+   0, 7680+ 128, 3072+   0, 7680+ 256,  128);
		API_polar::template f  <  64>(   l, 7936+   0, 7936+  64,            7936+ 128,   64);
		API_polar::template f  <  32>(   l, 8064+   0, 8064+  32,            8064+  64,   32);
		API_polar::template f  <  16>(   l, 8128+   0, 8128+  16,            8128+  32,   16);
		re9re10r110(8160, 3200);
		API_polar::template g  <  16>(s, l, 8128+   0, 8128+  16, 3200+   0, 8128+  32,   16);
		s8(8160, 3216);
		API_polar::template xo <  16>(s,    3200+   0, 3200+  16,            3200+   0,   16);
		API_polar::template g  <  32>(s, l, 8064+   0, 8064+  32, 3200+   0, 8064+  64,   32);
		s7(8128, 3232);
		API_polar::template xo <  32>(s,    3200+   0, 3200+  32,            3200+   0,   32);
		API_polar::template g  <  64>(s, l, 7936+   0, 7936+  64, 3200+   0, 7936+ 128,   64);
		s6(8064, 3264);
		API_polar::template xo <  64>(s,    3200+   0, 3200+  64,            3200+   0,   64);
		API_polar::template xo < 128>(s,    3072+   0, 3072+ 128,            3072+   0,  128);
		API_polar::template g  < 256>(s, l, 7168+   0, 7168+ 256, 3072+   0, 7168+ 512,  256);
		API_polar::template f  < 128>(   l, 7680+   0, 7680+ 128,            7680+ 256,  128);
		API_polar::template f  <  64>(   l, 7936+   0, 7936+  64,            7936+ 128,   64);
		API_polar::template f  <  32>(   l, 8064+   0, 8064+  32,            8064+  64,   32);
		r010s10s9s8(8128, 3328);
		API_polar::template g  <  32>(s, l, 8064+   0, 8064+  32, 3328+   0, 8064+  64,   32);
		s7(8128, 3360);
		API_polar::template xo <  32>(s,    3328+   0, 3328+  32,            3328+   0,   32);
		API_polar::template g  <  64>(s, l, 7936+   0, 7936+  64, 3328+   0, 7936+ 128,   64);
		s6(8064, 3392);
		API_polar::template xo <  64>(s,    3328+   0, 3328+  64,            3328+   0,   64);
		API_polar::template g  < 128>(s, l, 7680+   0, 7680+ 128, 3328+   0, 7680+ 256,  128);
		s5(7936, 3456);
		API_polar::template xo < 128>(s,    3328+   0, 3328+ 128,            3328+   0,  128);
		API_polar::template xo < 256>(s,    3072+   0, 3072+ 256,            3072+   0,  256);
		API_polar::template g  < 512>(s, l, 6144+   0, 6144+ 512, 3072+   0, 6144+1024,  512);
		API_polar::template f  < 256>(   l, 7168+   0, 7168+ 256,            7168+ 512,  256);
		API_polar::template f  < 128>(   l, 7680+   0, 7680+ 128,            7680+ 256,  128);
		API_polar::template f  <  64>(   l, 7936+   0, 7936+  64,            7936+ 128,   64);
		re10s10s9s8s7(8064, 3584);
		API_polar::template g  <  64>(s, l, 7936+   0, 7936+  64, 3584+   0, 7936+ 128,   64);
		s6(8064, 3648);
		API_polar::template xo <  64>(s,    3584+   0, 3584+  64,            3584+   0,   64);
		API_polar::template g  < 128>(s, l, 7680+   0, 7680+ 128, 3584+   0, 7680+ 256,  128);
		s5(7936, 3712);
		API_polar::template xo < 128>(s,    3584+   0, 3584+ 128,            3584+   0,  128);
		API_polar::template g  < 256>(s, l, 7168+   0, 7168+ 256, 3584+   0, 7168+ 512,  256);
		API_polar::template spc< 256>(s, l, 7680+   0,                       3840+   0,  256);
		API_polar::template xo < 256>(s,    3584+   0, 3584+ 256,            3584+   0,  256);
		API_polar::template xo < 512>(s,    3072+   0, 3072+ 512,            3072+   0,  512);
		API_polar::template xo <1024>(s,    2048+   0, 2048+1024,            2048+   0, 1024);
		API_polar::template xo <2048>(s,       0+   0,    0+2048,               0+   0, 2048);
	}
};
}
}
#endif
