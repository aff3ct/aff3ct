#ifndef FACTORY_MODULATOR_HPP
#define FACTORY_MODULATOR_HPP

#include "Module/Modulator/Modulator.hpp"
#include "Tools/params.h"

#include "Factory.hpp"

namespace aff3ct
{
template <typename B, typename R, typename Q>
struct Factory_modulator : public Factory
{
	static Modulator<B,R,Q>* build(const parameters &params, const float sigma);
};
}

#endif /* FACTORY_MODULATOR_HPP */
