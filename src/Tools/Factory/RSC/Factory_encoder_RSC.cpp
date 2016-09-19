#include "Module/Encoder/RSC/Encoder_RSC3_sys.hpp"
#include "Module/Encoder/RSC/Encoder_RSC4_sys.hpp"

#include "Factory_encoder_RSC.hpp"

template <typename B>
Encoder_RSC_sys<B>* Factory_encoder_RSC<B>
::build(const parameters &params, const int n_frames)
{
	Encoder_RSC_sys<B> *encoder = nullptr;

	const auto N = (params.code.type.find("TURBO") != std::string::npos) ? 2*params.code.K : params.code.N;

	// build the encoder
	if (params.encoder.systematic)
	{
		if (params.decoder.type == "BCJR4" || params.decoder.type == "CCSDS")
			encoder = new Encoder_RSC4_sys<B>(params.code.K, N, n_frames, params.encoder.buffered);
		else
			encoder = new Encoder_RSC3_sys<B>(params.code.K, N, n_frames, params.encoder.buffered);
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
