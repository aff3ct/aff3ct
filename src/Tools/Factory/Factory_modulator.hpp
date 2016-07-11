#ifndef FACTORY_MODULATOR_HPP
#define FACTORY_MODULATOR_HPP

#include "../../Modulator/Modulator.hpp"

#include "../params.h"

template <typename B, typename R>
struct Factory_modulator
{
	static Modulator<B,R>* build(const t_mod_param &mod_params, const R sigma);
};

#endif /* FACTORY_MODULATOR_HPP */
