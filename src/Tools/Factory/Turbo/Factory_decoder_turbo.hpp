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
namespace tools
{
template <typename B = int, typename R = float>
struct Factory_decoder_turbo : public Factory
{
	static module::Decoder<B,R>* build(const parameters                 &params,
	                                   const module::Interleaver<short> *interleaver,
	                                         module::SISO<R>            *siso_n,
	                                         module::SISO<R>            *siso_i,
	                                         Scaling_factor<R>          *scaling_factor,
	                                         module::CRC<B>             *crc = nullptr);
};
}
}

#endif /* FACTORY_DECODER_TURBO_HPP */
