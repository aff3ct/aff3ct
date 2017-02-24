#ifndef FACTORY_FROZENBITS_GENERATOR_HPP
#define FACTORY_FROZENBITS_GENERATOR_HPP

#include "Tools/params.h"
#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator.hpp"

#include "../Factory.hpp"

namespace aff3ct
{
template <typename B>
struct Factory_frozenbits_generator : public Factory
{
	static Frozenbits_generator<B>* build(const parameters &params);
};
}

#endif /* FACTORY_FROZENBITS_GENERATOR_HPP */
