#include "Module/Encoder/RSC/Encoder_RSC3_sys.hpp"
#include "Module/Encoder/RSC/Encoder_RSC4_sys.hpp"

#include "Factory_encoder_RSC.hpp"

template <typename B>
Encoder_RSC_sys<B>* Factory_encoder_RSC<B>
::build(const t_simulation_param &simu_params,
        const t_code_param       &code_params,
        const t_encoder_param    &enco_params,
        const t_decoder_param    &deco_params,
        const int                 n_frames)
{
	Encoder_RSC_sys<B> *encoder = nullptr;

	const auto N = (code_params.type.find("TURBO") != std::string::npos) ? 2*code_params.K : code_params.N;

	// build the encoder
	if (enco_params.systematic)
	{
		if (deco_params.algo == "BCJR4" || deco_params.algo == "CCSDS")
			encoder = new Encoder_RSC4_sys<B>(code_params.K, N, n_frames, enco_params.buffered);
		else
			encoder = new Encoder_RSC3_sys<B>(code_params.K, N, n_frames, enco_params.buffered);
	}

	return encoder;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct Factory_encoder_RSC<B_8>;
template struct Factory_encoder_RSC<B_16>;
template struct Factory_encoder_RSC<B_32>;
template struct Factory_encoder_RSC<B_64>;
#else
template struct Factory_encoder_RSC<B>;
#endif
// ==================================================================================== explicit template instantiation
