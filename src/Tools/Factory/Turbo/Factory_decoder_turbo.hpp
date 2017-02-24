#ifndef FACTORY_DECODER_TURBO_HPP
#define FACTORY_DECODER_TURBO_HPP

#include <string>

#include "Module/Decoder/Decoder.hpp"
#include "Module/Decoder/SISO.hpp"
#include "Module/Interleaver/Interleaver.hpp"
#include "Module/CRC/CRC.hpp"
#include "Tools/Code/Turbo/Scaling_factor/Scaling_factor.hpp"
#include "Tools/params.h"

#include "../Factory.hpp"

namespace aff3ct
{
template <typename B, typename R>
struct Factory_decoder_turbo : public Factory
{
	static Decoder<B,R>* build(const parameters          &params,
	                           const Interleaver<short>  *interleaver,
	                                 SISO<R>             *siso_n, 
	                                 SISO<R>             *siso_i,
	                                 Scaling_factor<R>   *scaling_factor,
	                                 CRC<B>              *crc = nullptr);
};
}

#endif /* FACTORY_DECODER_TURBO_HPP */
