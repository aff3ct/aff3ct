#include <cmath>

#include "../../Encoder/Polar/Encoder_polar.hpp"
#include "../../Encoder/Polar/Encoder_polar_sys.hpp"

#include "Factory_encoder_polar.hpp"

template <typename B>
Encoder_polar<B>* Factory_encoder_polar<B>
::build(const t_code_param    &code_params,
        const t_encoder_param &enco_params,
        const t_decoder_param &deco_params,
        const mipp::vector<B> &frozen_bits,
        const int n_frames)
{
	Encoder_polar<B> *encoder = nullptr;

	const int N = std::exp2(code_params.m);

	// build the encoder
	if (!enco_params.systematic)
		encoder = new Encoder_polar<B>(code_params.K, N, frozen_bits, n_frames);
	else
		encoder = new Encoder_polar_sys<B>(code_params.K, N, frozen_bits, n_frames);

	return encoder;
}

// ==================================================================================== explicit template instantiation 
#include "../types.h"
#ifdef MULTI_PREC
template struct Factory_encoder_polar<B_8>;
template struct Factory_encoder_polar<B_16>;
template struct Factory_encoder_polar<B_32>;
template struct Factory_encoder_polar<B_64>;
#else
template struct Factory_encoder_polar<B>;
#endif
// ==================================================================================== explicit template instantiation