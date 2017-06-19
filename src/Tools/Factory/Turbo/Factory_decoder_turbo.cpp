#include "Tools/Exception/exception.hpp"

#include "Module/Decoder/Turbo/Decoder_turbo_naive.hpp"
#include "Module/Decoder/Turbo/Decoder_turbo_fast.hpp"

#include "Factory_decoder_turbo.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B, typename R>
Decoder_turbo<B,R>* Factory_decoder_turbo<B,R>
::build(const std::string       type,
        const std::string       implem,
        const int               K,
        const int               N,
        const int               n_ite,
        const Interleaver<int> &itl,
              SISO<R>          &siso_n,
              SISO<R>          &siso_i,
        const bool              buffered)
{
	if (type == "TURBO")
	{
		     if (implem == "STD" ) return new Decoder_turbo_naive<B,R>(K, N, n_ite, itl, siso_n, siso_i, buffered);
		else if (implem == "FAST") return new Decoder_turbo_fast <B,R>(K, N, n_ite, itl, siso_n, siso_i, buffered);
	}

	throw cannot_allocate(__FILE__, __LINE__, __func__);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct aff3ct::tools::Factory_decoder_turbo<B_8,Q_8>;
template struct aff3ct::tools::Factory_decoder_turbo<B_16,Q_16>;
template struct aff3ct::tools::Factory_decoder_turbo<B_32,Q_32>;
template struct aff3ct::tools::Factory_decoder_turbo<B_64,Q_64>;
#else
template struct aff3ct::tools::Factory_decoder_turbo<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
