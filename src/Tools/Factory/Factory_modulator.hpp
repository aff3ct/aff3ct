#ifndef FACTORY_MODULATOR_HPP
#define FACTORY_MODULATOR_HPP

#include "Module/Modulator/Modulator.hpp"
#include "Tools/params.h"

#include "Factory.hpp"

template <typename B, typename R, typename Q>
struct Factory_modulator : public Factory
{
	static Modulator<B,R,Q>* build(const parameters &params, const float sigma, mipp::vector<R> &H);
};

#endif /* FACTORY_MODULATOR_HPP */
