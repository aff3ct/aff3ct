#include <cmath>

#include "Module/Encoder/Polar/Encoder_polar.hpp"
#include "Module/Encoder/Polar/Encoder_polar_sys.hpp"

#include "Factory_encoder_polar.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B>
Encoder<B>* Factory_encoder_polar<B>
::build(const parameters &params, const mipp::vector<B> &frozen_bits, const int n_frames)
{
	Encoder<B> *encoder = nullptr;

	// build the encoder
	if (!params.encoder.systematic)
		encoder = new Encoder_polar    <B>(params.code.K, params.code.N_code, frozen_bits, n_frames);
	else
		encoder = new Encoder_polar_sys<B>(params.code.K, params.code.N_code, frozen_bits, n_frames);

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
