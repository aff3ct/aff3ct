#include <stdexcept>

#include "Module/Encoder/RA/Encoder_RA.hpp"

#include "Factory_encoder_RA.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B>
Encoder<B>* Factory_encoder_RA<B>
::build(const std::string       type,
        const int               K,
        const int               N,
        const Interleaver<int> &itl,
        const int               n_frames)
{
	if (type == "RA") return new Encoder_RA<B>(K, N, itl, n_frames);

	throw std::runtime_error("aff3ct::tools::Factory_encoder_RA: the factory could not allocate the object.");
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct aff3ct::tools::Factory_encoder_RA<B_8>;
template struct aff3ct::tools::Factory_encoder_RA<B_16>;
template struct aff3ct::tools::Factory_encoder_RA<B_32>;
template struct aff3ct::tools::Factory_encoder_RA<B_64>;
#else
template struct aff3ct::tools::Factory_encoder_RA<B>;
#endif
// ==================================================================================== explicit template instantiation
