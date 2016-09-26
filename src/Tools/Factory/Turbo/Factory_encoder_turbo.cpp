#include "Module/Encoder/Turbo/Encoder_turbo.hpp"
#include "Module/Encoder/Turbo/Encoder_turbo_legacy.hpp"

#include "Factory_encoder_turbo.hpp"

template <typename B>
Encoder<B>* Factory_encoder_turbo<B>
::build(const t_code_param        &code_params,
        const t_encoder_param     &enco_params,
        const Interleaver<short>  *interleaver,
              Encoder_sys<B>      *sub_encoder_n,
              Encoder_sys<B>      *sub_encoder_i,
        const int                 n_frames)
{
	Encoder<B> *encoder = nullptr;

	// build the encoder
	if (enco_params.systematic)
	{
		if (enco_params.buffered)
			encoder = new Encoder_turbo<B>(code_params.K, code_params.N, *interleaver, *sub_encoder_n, *sub_encoder_i, 
			                               n_frames);
		else
			encoder = new Encoder_turbo_legacy<B>(code_params.K, code_params.N, *interleaver, *sub_encoder_n, n_frames);
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
