#ifndef FACTORY_ENCODER_RSC_HPP
#define FACTORY_ENCODER_RSC_HPP

#include "../../Encoder/RSC/Encoder_RSC.hpp"

#include "../params.h"

template <typename B>
struct Factory_encoder_RSC
{
	static Encoder_RSC<B>* build(const t_simulation_param &simu_params,
	                             const t_code_param       &code_params,
	                             const t_encoder_param    &enco_params,
	                             const t_decoder_param    &deco_params,
	                             const int                 n_frames = 1);
};

#endif /* FACTORY_ENCODER_RSC_HPP */