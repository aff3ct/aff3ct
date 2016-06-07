#ifndef FACTORY_SCALING_FACTOR_HPP
#define FACTORY_SCALING_FACTOR_HPP

#include "../Turbo/Scaling_factor/Scaling_factor.hpp"

#include "../params.h"
#include "../MIPP/mipp.h"

template <typename R>
struct Factory_scaling_factor
{
	static Scaling_factor<R>* build(const t_code_param    &code_params,
	                                const t_decoder_param &deco_params);
};

#endif /* FACTORY_SCALING_FACTOR_HPP */