#include <string>

#include "Module/Decoder/Repetition/Decoder_repetition_std.hpp"
#include "Module/Decoder/Repetition/Decoder_repetition_fast.hpp"

#include "Factory_decoder_repetition.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B, typename R>
Decoder<B,R>* Factory_decoder_repetition<B,R>
::build(const parameters &params)
{
	Decoder<B,R> *decoder = nullptr;

	if (params.decoder.type == "REPETITION")
	{
		if (params.decoder.implem == "STD")
			decoder = new Decoder_repetition_std<B,R>(params.code.K,
			                                          params.code.N,
			                                          params.encoder.buffered,
			                                          params.simulation.inter_frame_level);
		else if (params.decoder.implem == "FAST")
			decoder = new Decoder_repetition_fast<B,R>(params.code.K,
			                                           params.code.N,
			                                           params.encoder.buffered,
			                                           params.simulation.inter_frame_level);
	}

	return decoder;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct Factory_decoder_repetition<B_8,Q_8>;
template struct Factory_decoder_repetition<B_16,Q_16>;
template struct Factory_decoder_repetition<B_32,Q_32>;
template struct Factory_decoder_repetition<B_64,Q_64>;
#else
template struct Factory_decoder_repetition<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
