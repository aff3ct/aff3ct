#ifndef FACTORY_PUNCTURER_POLAR_HPP
#define FACTORY_PUNCTURER_POLAR_HPP

#include "../../Puncturer/Puncturer.hpp"
#include "../Polar/Frozenbits_generator/Frozenbits_generator.hpp"

#include "../params.h"

template <typename B, typename Q>
struct Factory_puncturer_polar
{
	static Puncturer<B,Q>* build(const t_code_param &code_params, const t_decoder_param &deco_params, 
	                             const Frozenbits_generator<B> *fb_generator);
};

#endif /* FACTORY_PUNCTURER_POLAR_HPP */