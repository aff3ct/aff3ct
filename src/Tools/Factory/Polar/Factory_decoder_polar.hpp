#ifndef FACTORY_DECODER_POLAR_HPP
#define FACTORY_DECODER_POLAR_HPP

#include "Module/CRC/CRC.hpp"
#include "Module/Decoder/SISO.hpp"
#include "Module/Decoder/Decoder.hpp"

#include "../../MIPP/mipp.h"

#include "../../params.h"

template <typename B, typename R>
struct Factory_decoder_polar
{
	static SISO<R>* build_siso(const t_code_param    &code_params, 
	                           const t_encoder_param &enco_params, 
	                           const t_channel_param &chan_params,
	                           const t_decoder_param &deco_params,
	                           const mipp::vector<B> &frozen_bits);

	static Decoder<B,R>* build(const t_code_param       &code_params,
	                           const t_encoder_param    &enco_params,
	                           const t_channel_param    &chan_params,
	                           const t_decoder_param    &deco_params,
	                           const mipp::vector<B>    &frozen_bits,
	                           CRC<B> *crc);
};

#endif /* FACTORY_DECODER_POLAR_HPP */
