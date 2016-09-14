#ifndef FACTORY_MODULATOR_HPP
#define FACTORY_MODULATOR_HPP

#include "Tools/params.h"

#include "Module/Modulator/Modulator.hpp"

template <typename B, typename R, typename Q>
struct Factory_modulator
{
	static Modulator<B,R,Q>* build(const parameters &params, const float sigma);
};

#endif /* FACTORY_MODULATOR_HPP */
