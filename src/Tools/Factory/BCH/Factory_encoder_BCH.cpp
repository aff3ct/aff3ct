#include "Factory_encoder_BCH.hpp"

#include "Module/Encoder/BCH/Encoder_BCH.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B>
Encoder<B>* Factory_encoder_BCH<B>
::build(const parameters &params, const tools::Galois &GF)
{
	Encoder<B> *encoder = nullptr;

	// build the encoder
	if (params.encoder.systematic)
		encoder = new Encoder_BCH<B>(params.code.K, params.code.N, params.code.m, GF);

	return encoder;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct aff3ct::tools::Factory_encoder_BCH<B_8>;
template struct aff3ct::tools::Factory_encoder_BCH<B_16>;
template struct aff3ct::tools::Factory_encoder_BCH<B_32>;
template struct aff3ct::tools::Factory_encoder_BCH<B_64>;
#else
template struct aff3ct::tools::Factory_encoder_BCH<B>;
#endif
// ==================================================================================== explicit template instantiation
