#include "Module/Encoder/Turbo/Encoder_turbo.hpp"
#include "Module/Encoder/Turbo/Encoder_turbo_legacy.hpp"

#include "Factory_encoder_turbo.hpp"
using namespace aff3ct;

template <typename B>
Encoder<B>* Factory_encoder_turbo<B>
::build(const parameters         &params,
              Interleaver<short> *interleaver,
              Encoder_sys<B>     *sub_encoder_n,
              Encoder_sys<B>     *sub_encoder_i,
        const int                 n_frames)
{
	Encoder<B> *encoder = nullptr;

	// build the encoder
	if (params.encoder.systematic)
	{
		if (params.encoder.buffered)
			encoder = new Encoder_turbo<B>(params.code.K, params.code.N, *interleaver, *sub_encoder_n, *sub_encoder_i,
			                               n_frames);
		else
			encoder = new Encoder_turbo_legacy<B>(params.code.K, params.code.N, *interleaver, *sub_encoder_n, n_frames);
	}

	return encoder;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct Factory_encoder_turbo<B_8>;
template struct Factory_encoder_turbo<B_16>;
template struct Factory_encoder_turbo<B_32>;
template struct Factory_encoder_turbo<B_64>;
#else
template struct Factory_encoder_turbo<B>;
#endif
// ==================================================================================== explicit template instantiation
