#include <string>

#include "Module/Decoder/Repetition/Decoder_repetition_std.hpp"
#include "Module/Decoder/Repetition/Decoder_repetition_fast.hpp"

#include "Factory_decoder_repetition.hpp"

template <typename B, typename R>
Decoder<B,R>* Factory_decoder_repetition<B,R>
::build(const t_code_param    &code_params,
        const t_encoder_param &enco_params,
        const t_decoder_param &deco_params)
{
	Decoder<B,R> *decoder = nullptr;

	if (deco_params.algo == "REPETITION")
	{
		if (deco_params.implem == "STD")
			decoder = new Decoder_repetition_std<B,R>(code_params.K, code_params.N, enco_params.buffered);
		else if (deco_params.implem == "FAST")
			decoder = new Decoder_repetition_fast<B,R>(code_params.K, code_params.N, enco_params.buffered);
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
