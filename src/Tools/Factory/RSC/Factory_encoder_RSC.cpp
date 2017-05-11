#include "Module/Encoder/RSC/Encoder_RSC_generic_sys.hpp"
#include "Module/Encoder/RSC/Encoder_RSC_generic_json_sys.hpp"

#include "Factory_encoder_RSC.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B>
Encoder_RSC_sys<B>* Factory_encoder_RSC<B>
::build(const parameters &params, std::ostream &stream)
{
	Encoder_RSC_sys<B> *encoder = nullptr;

	const auto N = (params.code.type.find("TURBO") != std::string::npos) ? 2*params.code.K + params.code.tail_length/2 : params.code.N;

	// build the encoder
	if (params.encoder.systematic)
	{
		if (params.encoder.type == "TURBO_JSON")
		{
			encoder = new Encoder_RSC_generic_json_sys<B>(params.code.K, N, params.simulation.inter_frame_level, params.encoder.buffered,
			                                              params.encoder.poly, stream);
		}
		else
		{
			encoder = new Encoder_RSC_generic_sys<B>(params.code.K, N, params.simulation.inter_frame_level, params.encoder.buffered,
			                                         params.encoder.poly);
		}
	}

	return encoder;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct aff3ct::tools::Factory_encoder_RSC<B_8>;
template struct aff3ct::tools::Factory_encoder_RSC<B_16>;
template struct aff3ct::tools::Factory_encoder_RSC<B_32>;
template struct aff3ct::tools::Factory_encoder_RSC<B_64>;
#else
template struct aff3ct::tools::Factory_encoder_RSC<B>;
#endif
// ==================================================================================== explicit template instantiation
