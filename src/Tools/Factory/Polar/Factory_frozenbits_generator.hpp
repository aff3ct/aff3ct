#ifndef FACTORY_FROZENBITS_GENERATOR_HPP
#define FACTORY_FROZENBITS_GENERATOR_HPP

#include "Tools/params.h"
#include "Tools/Polar/Frozenbits_generator/Frozenbits_generator.hpp"

template <typename B>
struct Factory_frozenbits_generator
{
	static Frozenbits_generator<B>* build(const t_simulation_param &simu_params, const t_code_param &code_params);
};

#endif /* FACTORY_FROZENBITS_GENERATOR_HPP */
