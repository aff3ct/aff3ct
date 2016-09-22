#ifndef FACTORY_ENCODER_LDPC_HPP
#define FACTORY_ENCODER_LDPC_HPP

#include "Module/Encoder/LDPC/Encoder_LDPC_sys.hpp"
#include "Tools/params.h"

#include "../Factory.hpp"

template <typename B>
struct Factory_encoder_LDPC : public Factory
{
	static Encoder_LDPC_sys<B>* build(const parameters &params, const int coset_seed = 0);
};

#endif /* FACTORY_ENCODER_LDPC_HPP */
