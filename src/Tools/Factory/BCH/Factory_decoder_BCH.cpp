#include "Module/Decoder/BCH/Decoder_BCH.hpp"

#include "Factory_decoder_BCH.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B, typename R>
Decoder<B,R>* Factory_decoder_BCH<B,R>
::build(const std::string  type,
        const std::string  implem,
        const int          K,
        const int          N,
        const int          t,
        const Galois      &GF,
        const int          n_frames)
{
	if (type == "ALGEBRAIC")
	{
		if (implem == "STD") return new Decoder_BCH<B,R>(K, N, t, GF, n_frames);
	}

	return nullptr;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct aff3ct::tools::Factory_decoder_BCH<B_8,Q_8>;
template struct aff3ct::tools::Factory_decoder_BCH<B_16,Q_16>;
template struct aff3ct::tools::Factory_decoder_BCH<B_32,Q_32>;
template struct aff3ct::tools::Factory_decoder_BCH<B_64,Q_64>;
#else
template struct aff3ct::tools::Factory_decoder_BCH<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
