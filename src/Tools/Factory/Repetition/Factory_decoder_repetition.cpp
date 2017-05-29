#include "Module/Decoder/Repetition/Decoder_repetition_std.hpp"
#include "Module/Decoder/Repetition/Decoder_repetition_fast.hpp"

#include "Factory_decoder_repetition.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B, typename R>
Decoder<B,R>* Factory_decoder_repetition<B,R>
::build(const std::string type,
        const std::string implem,
        const int         K,
        const int         N,
        const bool        buffered,
        const int         n_frames)
{
	if (type == "REPETITION")
	{
		     if (implem == "STD" ) return new Decoder_repetition_std <B,R>(K, N, buffered, n_frames);
		else if (implem == "FAST") return new Decoder_repetition_fast<B,R>(K, N, buffered, n_frames);
	}

	return nullptr;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct aff3ct::tools::Factory_decoder_repetition<B_8,Q_8>;
template struct aff3ct::tools::Factory_decoder_repetition<B_16,Q_16>;
template struct aff3ct::tools::Factory_decoder_repetition<B_32,Q_32>;
template struct aff3ct::tools::Factory_decoder_repetition<B_64,Q_64>;
#else
template struct aff3ct::tools::Factory_decoder_repetition<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
