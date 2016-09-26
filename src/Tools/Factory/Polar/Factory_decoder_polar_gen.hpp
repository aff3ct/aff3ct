#ifndef FACTORY_DECODER_POLAR_GEN_HPP
#define FACTORY_DECODER_POLAR_GEN_HPP

#include "Module/Decoder/Decoder.hpp"

#include "Tools/MIPP/mipp.h"
#include "Tools/params.h"

template <typename B, typename R>
struct Factory_decoder_polar_gen
{
	static Decoder<B,R>* build(const t_code_param       &code_params,
	                           const t_encoder_param    &enco_params,
	                           const t_channel_param    &chan_params,
	                           const t_decoder_param    &deco_params,
	                           const mipp::vector<B>    &frozen_bits);

	static void get_frozen_bits(const t_code_param     &code_params, 
	                            const t_decoder_param  &deco_params, 
	                                  mipp::vector<B>  &frozen_bits);
};

#endif /* FACTORY_DECODER_POLAR_GEN_HPP */
