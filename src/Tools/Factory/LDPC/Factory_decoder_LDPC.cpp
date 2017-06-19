#include "Tools/Exception/exception.hpp"

#include "Module/Decoder/LDPC/BP/Flooding/SPA/Decoder_LDPC_BP_flooding_sum_product.hpp"
#include "Module/Decoder/LDPC/BP/Flooding/LSPA/Decoder_LDPC_BP_flooding_log_sum_product.hpp"
#include "Module/Decoder/LDPC/BP/Flooding/ONMS/Decoder_LDPC_BP_flooding_offset_normalize_min_sum.hpp"
#include "Module/Decoder/LDPC/BP/Flooding/Gallager/Decoder_LDPC_BP_flooding_Gallager_A.hpp"
#include "Module/Decoder/LDPC/BP/Layered/SPA/Decoder_LDPC_BP_layered_sum_product.hpp"
#include "Module/Decoder/LDPC/BP/Layered/LSPA/Decoder_LDPC_BP_layered_log_sum_product.hpp"
#include "Module/Decoder/LDPC/BP/Layered/ONMS/Decoder_LDPC_BP_layered_offset_normalize_min_sum.hpp"
#include "Module/Decoder/LDPC/BP/Layered/ONMS/Decoder_LDPC_BP_layered_ONMS_inter.hpp"

#include "Factory_decoder_LDPC.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B, typename R>
Decoder_SISO<B,R>* Factory_decoder_LDPC<B,R>
::build(const std::string            type,
        const std::string            implem,
        const int                    K,
        const int                    N,
        const int                    n_ite,
        const Sparse_matrix         &H,
        const std::vector<unsigned> &info_bits_pos,
        const std::string            simd_strategy,
        const float                  factor,
        const R                      offset,
        const bool                   synd,
        const int                    synd_depth,
        const int                    n_frames)
{
	if ((type == "BP" || type == "BP_FLOODING") && simd_strategy.empty())
	{
		     if (implem == "ONMS") return new Decoder_LDPC_BP_flooding_ONMS     <B,R>(K, N, n_ite, H, info_bits_pos, factor, offset, synd, synd_depth, n_frames);
		else if (implem == "GALA") return new Decoder_LDPC_BP_flooding_GALA     <B,R>(K, N, n_ite, H, info_bits_pos,                 synd, synd_depth, n_frames);
		else if (implem == "SPA" ) return new Decoder_LDPC_BP_flooding_SPA      <B,R>(K, N, n_ite, H, info_bits_pos,                 synd, synd_depth, n_frames);
		else if (implem == "LSPA") return new Decoder_LDPC_BP_flooding_LSPA     <B,R>(K, N, n_ite, H, info_bits_pos,                 synd, synd_depth, n_frames);
	}
	else if (type == "BP_LAYERED" && simd_strategy.empty())
	{
		     if (implem == "ONMS") return new Decoder_LDPC_BP_layered_ONMS      <B,R>(K, N, n_ite, H, info_bits_pos, factor, offset, synd, synd_depth, n_frames);
		else if (implem == "SPA" ) return new Decoder_LDPC_BP_layered_SPA       <B,R>(K, N, n_ite, H, info_bits_pos,                 synd, synd_depth, n_frames);
		else if (implem == "LSPA") return new Decoder_LDPC_BP_layered_LSPA      <B,R>(K, N, n_ite, H, info_bits_pos,                 synd, synd_depth, n_frames);
	}
	else if (type == "BP_LAYERED" && simd_strategy == "INTER")
	{
		     if (implem == "ONMS") return new Decoder_LDPC_BP_layered_ONMS_inter<B,R>(K, N, n_ite, H, info_bits_pos, factor, offset, synd, synd_depth, n_frames);
	}

	throw cannot_allocate(__FILE__, __LINE__, __func__);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct aff3ct::tools::Factory_decoder_LDPC<B_8,Q_8>;
template struct aff3ct::tools::Factory_decoder_LDPC<B_16,Q_16>;
template struct aff3ct::tools::Factory_decoder_LDPC<B_32,Q_32>;
template struct aff3ct::tools::Factory_decoder_LDPC<B_64,Q_64>;
#else
template struct aff3ct::tools::Factory_decoder_LDPC<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
