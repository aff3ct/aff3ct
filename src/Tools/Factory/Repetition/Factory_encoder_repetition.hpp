#ifndef FACTORY_ENCODER_REPETITION_HPP
#define FACTORY_ENCODER_REPETITION_HPP

#include "Module/Encoder/Encoder_sys.hpp"

#include "Tools/params.h"

template <typename B>
struct Factory_encoder_repetition
{
	static Encoder_sys<B>* build(const t_simulation_param &simu_params,
	                             const t_code_param       &code_params,
	                             const t_encoder_param    &enco_params);
};

#endif /* FACTORY_ENCODER_REPETITION_HPP */
