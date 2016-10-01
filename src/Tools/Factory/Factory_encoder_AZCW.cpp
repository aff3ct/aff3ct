#include "Module/Encoder/AZCW/Encoder_AZCW.hpp"

#include "Factory_encoder_AZCW.hpp"

template <typename B>
Encoder_sys<B>* Factory_encoder_AZCW<B>
::build(const parameters &params)
{
	Encoder_sys<B> *encoder = nullptr;

	// build the encoder
	encoder = new Encoder_AZCW<B>(params.code.K, params.code.N_code);

	return encoder;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct Factory_encoder_AZCW<B_8 >;
template struct Factory_encoder_AZCW<B_16>;
template struct Factory_encoder_AZCW<B_32>;
template struct Factory_encoder_AZCW<B_64>;
#else
template struct Factory_encoder_AZCW<B>;
#endif
// ==================================================================================== explicit template instantiation
