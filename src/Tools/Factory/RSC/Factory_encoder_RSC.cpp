#include "Module/Encoder/RSC/Encoder_RSC_generic_sys.hpp"
#include "Module/Encoder/RSC/Encoder_RSC_generic_json_sys.hpp"
#include "Factory_encoder_RSC.hpp"

template <typename B>
Encoder_RSC_sys<B>* Factory_encoder_RSC<B>
::build(const parameters &params, const int n_frames, std::ostream &stream)
{
	Encoder_RSC_sys<B> *encoder = nullptr;

	const auto N = (params.code.type.find("TURBO") != std::string::npos) ? 2*params.code.K : params.code.N;

	// build the encoder
	if (params.encoder.systematic)
	{
		if (params.encoder.type == "TURBO_JSON")
		{
			encoder = new Encoder_RSC_generic_json_sys<B>(params.code.K, N, n_frames, params.encoder.buffered,
			                                              params.encoder.poly, stream);
		}
		else
		{
			encoder = new Encoder_RSC_generic_sys<B>(params.code.K, N, n_frames, params.encoder.buffered,
			                                         params.encoder.poly);
		}
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
