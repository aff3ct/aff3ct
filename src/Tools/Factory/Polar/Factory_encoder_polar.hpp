#ifndef FACTORY_ENCODER_POLAR_HPP
#define FACTORY_ENCODER_POLAR_HPP

#include "Tools/params.h"
#include "Tools/MIPP/mipp.h"

#include "Module/Encoder/Encoder.hpp"

template <typename B>
struct Factory_encoder_polar
{
	static Encoder<B>* build(const t_code_param    &code_params,
	                         const t_encoder_param &enco_params,
	                         const t_decoder_param &deco_params,
	                         const mipp::vector<B> &frozen_bits,
	                         const int n_frames = 1);
};

#endif /* FACTORY_ENCODER_POLAR_HPP */
