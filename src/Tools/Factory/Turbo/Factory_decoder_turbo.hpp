#ifndef FACTORY_DECODER_TURBO_HPP
#define FACTORY_DECODER_TURBO_HPP

#include <string>

#include "Tools/Code/Turbo/Scaling_factor/Scaling_factor.hpp"

#include "Module/Decoder/Decoder.hpp"
#include "Module/Decoder/SISO.hpp"
#include "Module/Interleaver/Interleaver.hpp"
#include "Module/CRC/CRC.hpp"

#include "../Factory.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename R = float>
struct Factory_decoder_turbo : public Factory
{
	static module::Decoder<B,R>* build(const std::string               type,
	                                   const int                       K,
	                                   const int                       N,
	                                   const int                       n_ite,
	                                   const module::Interleaver<int> &itl,
	                                         module::SISO<R>          &siso_n,
	                                         module::SISO<R>          &siso_i,
	                                         Scaling_factor<R>        &sf,
	                                         module::CRC<B>           *crc          = nullptr,
	                                   const int                       fnc_q        = 10,
	                                   const int                       fnc_ite_min  = 3,
	                                   const int                       fnc_ite_max  = -1,
	                                   const int                       fnc_ite_step = 1,
	                                   const bool                      buffered     = true);
};
}
}

#endif /* FACTORY_DECODER_TURBO_HPP */
