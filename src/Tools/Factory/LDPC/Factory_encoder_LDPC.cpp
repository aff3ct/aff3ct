#include "Tools/Factory/LDPC/Factory_encoder_LDPC.hpp"
#include "Module/Encoder/LDPC/Encoder_LDPC.hpp"


using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B>
Encoder<B>* Factory_encoder_LDPC<B>
::build(const parameters &params, const int seed, const int n_frames)
{
	Encoder<B> *encoder = nullptr;

	// build the encoder
	if (params.encoder.type == "LDPC")
		encoder = new Encoder_LDPC<B>(params.code.K, params.code.N + params.code.tail_length, AList_reader(params.encoder.path), n_frames);

	return encoder;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct aff3ct::tools::Factory_encoder_LDPC<B_8>;
template struct aff3ct::tools::Factory_encoder_LDPC<B_16>;
template struct aff3ct::tools::Factory_encoder_LDPC<B_32>;
template struct aff3ct::tools::Factory_encoder_LDPC<B_64>;
#else
template struct aff3ct::tools::Factory_encoder_LDPC<B>;
#endif
// ==================================================================================== explicit template instantiation
