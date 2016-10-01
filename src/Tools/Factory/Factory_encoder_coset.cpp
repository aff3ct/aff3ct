#include "Module/Encoder/Coset/Encoder_coset.hpp"

#include "Factory_encoder_coset.hpp"

template <typename B>
Encoder_sys<B>* Factory_encoder_coset<B>
::build(const parameters &params, const int seed)
{
	Encoder_sys<B> *encoder = nullptr;

	// build the encoder
	encoder = new Encoder_coset<B>(params.code.K, params.code.N_code, seed);

	return encoder;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct Factory_encoder_coset<B_8 >;
template struct Factory_encoder_coset<B_16>;
template struct Factory_encoder_coset<B_32>;
template struct Factory_encoder_coset<B_64>;
#else
template struct Factory_encoder_coset<B>;
#endif
// ==================================================================================== explicit template instantiation
