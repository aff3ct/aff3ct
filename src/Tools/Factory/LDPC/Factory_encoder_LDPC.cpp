#include "Tools/Exception/cannot_allocate.hpp"

#include "Module/Encoder/LDPC/Encoder_LDPC.hpp"
#include "Module/Encoder/LDPC/From_H/Encoder_LDPC_from_H.hpp"
#include "Module/Encoder/LDPC/DVBS2/Encoder_LDPC_DVBS2.hpp"

#include "Factory_encoder_LDPC.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B>
Encoder_LDPC<B>* Factory_encoder_LDPC<B>
::build(const std::string    type,
        const int            K,
        const int            N,
        const Sparse_matrix &G,
        const Sparse_matrix &H,
        const int            n_frames)
{
	     if (type == "LDPC"      ) return new Encoder_LDPC       <B>(K, N, G, n_frames);
	else if (type == "LDPC_H"    ) return new Encoder_LDPC_from_H<B>(K, N, H, n_frames);
	else if (type == "LDPC_DVBS2") return new Encoder_LDPC_DVBS2 <B>(K, N,    n_frames);

	throw cannot_allocate(__FILE__, __LINE__, __func__);
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
