#include <cmath>

#include "Module/Encoder/Polar/Encoder_polar.hpp"
#include "Module/Encoder/Polar/Encoder_polar_sys.hpp"

#include "Factory_encoder_polar.hpp"

template <typename B>
Encoder<B>* Factory_encoder_polar<B>
::build(const t_code_param    &code_params,
        const t_encoder_param &enco_params,
        const t_decoder_param &deco_params,
        const mipp::vector<B> &frozen_bits,
        const int n_frames)
{
	Encoder<B> *encoder = nullptr;

	// build the encoder
	if (!enco_params.systematic)
		encoder = new Encoder_polar    <B>(code_params.K, code_params.N_code, frozen_bits, n_frames);
	else
		encoder = new Encoder_polar_sys<B>(code_params.K, code_params.N_code, frozen_bits, n_frames);

	return encoder;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct Factory_encoder_polar<B_8>;
template struct Factory_encoder_polar<B_16>;
template struct Factory_encoder_polar<B_32>;
template struct Factory_encoder_polar<B_64>;
#else
template struct Factory_encoder_polar<B>;
#endif
// ==================================================================================== explicit template instantiation
