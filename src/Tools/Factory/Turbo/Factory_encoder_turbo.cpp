#include "Module/Encoder/Turbo/Encoder_turbo.hpp"
#include "Module/Encoder/Turbo/Encoder_turbo_legacy.hpp"

#include "Factory_encoder_turbo.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B>
Encoder<B>* Factory_encoder_turbo<B>
::build(const parameters         &params,
              Interleaver<int>   *interleaver,
              Encoder_sys<B>     *sub_encoder_n,
              Encoder_sys<B>     *sub_encoder_i)
{
	Encoder<B> *encoder = nullptr;

	// build the encoder
	if (params.encoder.systematic)
	{
		if (params.encoder.buffered)
			encoder = new Encoder_turbo<B>(params.code.K, params.code.N, *interleaver, *sub_encoder_n, *sub_encoder_i,
			                               params.simulation.inter_frame_level);
		else
			encoder = new Encoder_turbo_legacy<B>(params.code.K, params.code.N, *interleaver, *sub_encoder_n,
			                                      params.simulation.inter_frame_level);
	}

	return encoder;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct aff3ct::tools::Factory_encoder_turbo<B_8>;
template struct aff3ct::tools::Factory_encoder_turbo<B_16>;
template struct aff3ct::tools::Factory_encoder_turbo<B_32>;
template struct aff3ct::tools::Factory_encoder_turbo<B_64>;
#else
template struct aff3ct::tools::Factory_encoder_turbo<B>;
#endif
// ==================================================================================== explicit template instantiation
