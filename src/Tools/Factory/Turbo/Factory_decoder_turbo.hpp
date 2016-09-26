#ifndef FACTORY_DECODER_TURBO_HPP
#define FACTORY_DECODER_TURBO_HPP

#include <string>

#include "Tools/Turbo/Scaling_factor/Scaling_factor.hpp"
#include "Tools/params.h"

#include "Module/Decoder/Decoder.hpp"
#include "Module/Decoder/SISO.hpp"
#include "Module/Interleaver/Interleaver.hpp"
#include "Module/CRC/CRC.hpp"

template <typename B, typename R>
struct Factory_decoder_turbo
{
	static Decoder<B,R>* build(const t_code_param        &code_params,
	                           const t_encoder_param     &enco_params,
	                           const t_channel_param     &chan_params,
	                           const t_decoder_param     &deco_params,
	                           const Interleaver<short>  *interleaver,
	                                 SISO<R>             *siso_n, 
	                                 SISO<R>             *siso_i,
	                                 Scaling_factor<R>   *scaling_factor,
	                                 CRC<B>              *crc = nullptr);
};

#endif /* FACTORY_DECODER_TURBO_HPP */
