#ifndef FACTORY_DECODER_RSC_HPP
#define FACTORY_DECODER_RSC_HPP

#include "Tools/params.h"

#include "Module/Decoder/Decoder.hpp"
#include "Module/Decoder/SISO.hpp"

template <typename B, typename R, typename RD>
struct Factory_decoder_RSC
{
	static SISO<R>* build_siso(const t_code_param                  &code_params,
	                           const t_encoder_param               &enco_params,
	                           const t_channel_param               &chan_params,
	                           const t_decoder_param               &deco_params,
	                           const std::vector<std::vector<int>> &trellis);

	static Decoder<B,R>* build(const t_code_param                  &code_params,
	                           const t_encoder_param               &enco_params,
	                           const t_channel_param               &chan_params,
	                           const t_decoder_param               &deco_params,
	                           const std::vector<std::vector<int>> &trellis);
};

#endif /* FACTORY_DECODER_RSC_HPP */
