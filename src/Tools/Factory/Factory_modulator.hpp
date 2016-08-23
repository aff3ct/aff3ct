#ifndef FACTORY_MODULATOR_HPP
#define FACTORY_MODULATOR_HPP

#include "../../Modulator/Modulator.hpp"

#include "../params.h"

template <typename B, typename R, typename Q>
struct Factory_modulator
{
	static Modulator<B,R,Q>* build(const t_code_param &code_params, const t_mod_param &mod_params, const float sigma);
};

#endif /* FACTORY_MODULATOR_HPP */