#include "Tools/Exceptions/Cannot_allocate.hpp"

#include "Module/Encoder/Turbo/Encoder_turbo.hpp"
#include "Module/Encoder/Turbo/Encoder_turbo_legacy.hpp"

#include "Factory_encoder_turbo.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B>
Encoder<B>* Factory_encoder_turbo<B>
::build(const std::string       type,
        const int               K,
        const int               N,
        const Interleaver<int> &itl,
              Encoder_sys<B>   *enc_n,
              Encoder_sys<B>   *enc_i,
        const bool              buffered,
        const int               n_frames)
{
	enc_i = (enc_i == nullptr) ? enc_n : enc_i;

	if (buffered)
	{
		if (type == "TURBO") return new Encoder_turbo       <B>(K, N, itl, *enc_n, *enc_i, n_frames);
	}
	else if (enc_n == enc_i)
	{
		if (type == "TURBO") return new Encoder_turbo_legacy<B>(K, N, itl, *enc_n,         n_frames);
	}

	throw Cannot_allocate("aff3ct::tools::Factory_encoder_turbo: the factory could not allocate the object.");
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
