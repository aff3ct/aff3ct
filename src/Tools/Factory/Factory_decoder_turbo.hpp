#ifndef FACTORY_DECODER_TURBO_HPP
#define FACTORY_DECODER_TURBO_HPP

#include <string>

#include "../../Decoder/Decoder.hpp"
#include "../../Decoder/SISO.hpp"
#include "../../Interleaver/Interleaver.hpp"
#include "../../CRC/CRC.hpp"

#include "../Turbo/Scaling_factor/Scaling_factor.hpp"

#include "../params.h"

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