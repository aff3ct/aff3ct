#ifndef FACTORY_ENCODER_COSET_HPP
#define FACTORY_ENCODER_COSET_HPP

#include "Module/Encoder/Encoder_sys.hpp"
#include "Tools/params.h"

#include "Factory.hpp"

template <typename B>
struct Factory_encoder_coset : public Factory
{
	static Encoder_sys<B>* build(const parameters &params, const int seed = 0);
};

#endif /* FACTORY_ENCODER_COSET_HPP */
