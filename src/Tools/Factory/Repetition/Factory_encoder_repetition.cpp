#include "Module/Encoder/Repetition/Encoder_repetition_sys.hpp"

#include "Factory_encoder_repetition.hpp"

template <typename B>
Encoder_sys<B>* Factory_encoder_repetition<B>
::build(const t_simulation_param &simu_params,
        const t_code_param       &code_params,
        const t_encoder_param    &enco_params)
{
	Encoder_sys<B> *encoder = nullptr;

	// build the encoder
	if (enco_params.systematic)
		encoder = new Encoder_repetition_sys<B>(code_params.K, code_params.N, enco_params.buffered);

	return encoder;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct Factory_encoder_repetition<B_8>;
template struct Factory_encoder_repetition<B_16>;
template struct Factory_encoder_repetition<B_32>;
template struct Factory_encoder_repetition<B_64>;
#else
template struct Factory_encoder_repetition<B>;
#endif
// ==================================================================================== explicit template instantiation
