#ifndef FACTORY_ENCODER_POLAR_HPP
#define FACTORY_ENCODER_POLAR_HPP

#include "../../Encoder/Encoder.hpp"

#include "../params.h"
#include "../MIPP/mipp.h"

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