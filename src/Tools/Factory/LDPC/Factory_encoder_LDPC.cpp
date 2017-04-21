#include "Tools/Factory/LDPC/Factory_encoder_LDPC.hpp"
#include "Module/Encoder/LDPC/Encoder_LDPC.hpp"
#include "Module/Encoder/LDPC/From_H/Encoder_LDPC_from_H.hpp"
#include "Module/Encoder/LDPC/DVBS2/Encoder_LDPC_DVBS2.hpp"


using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B>
Encoder_LDPC<B>* Factory_encoder_LDPC<B>
::build(const parameters &params, const int seed)
{
	Encoder_LDPC<B> *encoder = nullptr;

	// build the encoder
	if (params.encoder.systematic)
	{
		if (params.encoder.type == "LDPC")
			encoder = new Encoder_LDPC<B>(params.code.K, params.code.N + params.code.tail_length, AList_reader(params.encoder.path), params.simulation.inter_frame_level);
		else if (params.encoder.type == "LDPC_H")
			encoder = new Encoder_LDPC_from_H<B>(params.code.K, params.code.N + params.code.tail_length, AList_reader(params.code.alist_path), params.simulation.inter_frame_level);
		else if (params.encoder.type == "LDPC_DVBS2")
			encoder = new Encoder_LDPC_DVBS2<B>(params.code.K, params.code.N + params.code.tail_length, params.simulation.inter_frame_level);
	}

	return encoder;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct aff3ct::tools::Factory_encoder_LDPC<B_8>;
template struct aff3ct::tools::Factory_encoder_LDPC<B_16>;
template struct aff3ct::tools::Factory_encoder_LDPC<B_32>;
template struct aff3ct::tools::Factory_encoder_LDPC<B_64>;
#else
template struct aff3ct::tools::Factory_encoder_LDPC<B>;
#endif
// ==================================================================================== explicit template instantiation
