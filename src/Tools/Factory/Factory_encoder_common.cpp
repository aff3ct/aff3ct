#include "Module/Encoder/NO/Encoder_NO.hpp"
#include "Module/Encoder/AZCW/Encoder_AZCW.hpp"
#include "Module/Encoder/Coset/Encoder_coset.hpp"
#include "Module/Encoder/User/Encoder_user.hpp"

#include "Factory_encoder_common.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B>
Encoder<B>* Factory_encoder_common<B>
::build(const parameters &params, const int seed, const int n_frames)
{
	Encoder<B> *encoder = nullptr;

	// build the encoder
	if (params.encoder.type == "NO")
		encoder = new Encoder_NO<B>(params.code.K, params.code.N + params.code.tail_length, n_frames);
	else if (params.encoder.type == "AZCW")
		encoder = new Encoder_AZCW<B>(params.code.K, params.code.N + params.code.tail_length, n_frames);
	else if (params.encoder.type == "COSET")
		encoder = new Encoder_coset<B>(params.code.K, params.code.N + params.code.tail_length, seed, n_frames);
	else if (params.encoder.type == "USER")
		encoder = new Encoder_user<B>(params.code.K, params.code.N + params.code.tail_length, params.encoder.path, n_frames);

	return encoder;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct Factory_encoder_common<B_8>;
template struct Factory_encoder_common<B_16>;
template struct Factory_encoder_common<B_32>;
template struct Factory_encoder_common<B_64>;
#else
template struct Factory_encoder_common<B>;
#endif
// ==================================================================================== explicit template instantiation
