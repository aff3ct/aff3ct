#include "Module/Encoder/LDPC/Encoder_LDPC_coset.hpp"
#include "Module/Encoder/LDPC/Encoder_LDPC_AZCW.hpp"

#include "Factory_encoder_LDPC.hpp"

template <typename B>
Encoder_LDPC_sys<B>* Factory_encoder_LDPC<B>
::build(const parameters &params, const int seed)
{
	Encoder_LDPC_sys<B> *encoder = nullptr;

	// build the encoder
	if (params.encoder.systematic)
	{
		if (params.code.coset)
			encoder = new Encoder_LDPC_coset<B>(params.code.K, params.code.N_code, seed);
		else if (params.source.type == "AZCW")
			encoder = new Encoder_LDPC_AZCW<B>(params.code.K, params.code.N_code);
	}

	return encoder;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct Factory_encoder_LDPC<B_8>;
template struct Factory_encoder_LDPC<B_16>;
template struct Factory_encoder_LDPC<B_32>;
template struct Factory_encoder_LDPC<B_64>;
#else
template struct Factory_encoder_LDPC<B>;
#endif
// ==================================================================================== explicit template instantiation
