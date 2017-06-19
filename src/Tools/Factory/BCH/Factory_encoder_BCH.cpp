#include "Tools/Exception/cannot_allocate.hpp"

#include "Module/Encoder/BCH/Encoder_BCH.hpp"

#include "Factory_encoder_BCH.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B>
Encoder<B>* Factory_encoder_BCH<B>
::build(const std::string  type,
        const int          K,
        const int          N,
        const Galois      &GF,
        const int          n_frames)
{
	if (type == "BCH") return new Encoder_BCH<B>(K, N, GF, n_frames);

	throw cannot_allocate(__FILE__, __LINE__, __func__);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct aff3ct::tools::Factory_encoder_BCH<B_8>;
template struct aff3ct::tools::Factory_encoder_BCH<B_16>;
template struct aff3ct::tools::Factory_encoder_BCH<B_32>;
template struct aff3ct::tools::Factory_encoder_BCH<B_64>;
#else
template struct aff3ct::tools::Factory_encoder_BCH<B>;
#endif
// ==================================================================================== explicit template instantiation
