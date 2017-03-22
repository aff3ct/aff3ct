#include "Module/Encoder/Repetition/Encoder_repetition_sys.hpp"

#include "Factory_encoder_repetition.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B>
Encoder_sys<B>* Factory_encoder_repetition<B>
::build(const parameters &params)
{
	Encoder_sys<B> *encoder = nullptr;

	// build the encoder
	if (params.encoder.systematic)
		encoder = new Encoder_repetition_sys<B>(params.code.K, params.code.N, params.encoder.buffered);

	return encoder;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct aff3ct::tools::Factory_encoder_repetition<B_8>;
template struct aff3ct::tools::Factory_encoder_repetition<B_16>;
template struct aff3ct::tools::Factory_encoder_repetition<B_32>;
template struct aff3ct::tools::Factory_encoder_repetition<B_64>;
#else
template struct aff3ct::tools::Factory_encoder_repetition<B>;
#endif
// ==================================================================================== explicit template instantiation
