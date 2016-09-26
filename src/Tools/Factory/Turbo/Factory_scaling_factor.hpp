#ifndef FACTORY_SCALING_FACTOR_HPP
#define FACTORY_SCALING_FACTOR_HPP

#include "Tools/params.h"
#include "Tools/Turbo/Scaling_factor/Scaling_factor.hpp"

template <typename R>
struct Factory_scaling_factor
{
	static Scaling_factor<R>* build(const t_code_param    &code_params,
	                                const t_decoder_param &deco_params);
};

#endif /* FACTORY_SCALING_FACTOR_HPP */
