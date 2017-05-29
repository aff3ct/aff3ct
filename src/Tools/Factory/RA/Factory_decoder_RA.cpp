#include "Module/Decoder/RA/Decoder_RA.hpp"

#include "Factory_decoder_RA.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B, typename R>
Decoder<B,R>* Factory_decoder_RA<B,R>
::build(const std::string               type,
        const std::string               implem,
        const int                       K,
        const int                       N,
        const module::Interleaver<int> &itl,
        const int                       n_ite,
        const int                       n_frames)
{
	if (type == "RA")
	{
		if (implem == "STD" ) return new Decoder_RA<B,R>(K, N, itl, n_ite, n_frames);
	}

	return nullptr;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct aff3ct::tools::Factory_decoder_RA<B_8,Q_8>;
template struct aff3ct::tools::Factory_decoder_RA<B_16,Q_16>;
template struct aff3ct::tools::Factory_decoder_RA<B_32,Q_32>;
template struct aff3ct::tools::Factory_decoder_RA<B_64,Q_64>;
#else
template struct aff3ct::tools::Factory_decoder_RA<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
