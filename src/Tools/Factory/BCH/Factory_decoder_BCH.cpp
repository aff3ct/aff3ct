#include <string>

#include "Module/Decoder/BCH/Decoder_BCH.hpp"

#include "Factory_decoder_BCH.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B, typename R>
Decoder<B,R>* Factory_decoder_BCH<B,R>
::build(const parameters &params, const Galois &GF)
{
	Decoder<B,R> *decoder = nullptr;

	if (params.decoder.type == "ALGEBRAIC")
		if (params.decoder.implem == "STD")
			decoder = new Decoder_BCH<B,R>(params.code.K,
			                               params.code.N,
			                               params.code.m,
			                               params.code.t,
			                               GF,
			                               params.simulation.inter_frame_level);

	return decoder;
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
