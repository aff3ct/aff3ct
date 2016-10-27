#include <string>

#include "Module/Decoder/LDPC/BP/Flooding/MSO/Decoder_LDPC_BP_flooding_min_sum_offset.hpp"
#include "Module/Decoder/LDPC/BP/Flooding/SPA/Decoder_LDPC_BP_flooding_sum_product.hpp"
#include "Module/Decoder/LDPC/BP/Flooding/LSPA/Decoder_LDPC_BP_flooding_log_sum_product.hpp"
#include "Module/Decoder/LDPC/BP/Layered/SPA/Decoder_LDPC_BP_layered_sum_product.hpp"
#include "Module/Decoder/LDPC/BP/Layered/LSPA/Decoder_LDPC_BP_layered_log_sum_product.hpp"
#include "Factory_decoder_LDPC.hpp"

template <typename B, typename R>
Decoder_SISO<B,R>* Factory_decoder_LDPC<B,R>
::build(const parameters &params, const AList_reader &alist_data)
{
	Decoder_SISO<B,R> *decoder = nullptr;

	if (params.decoder.type == "BP" || params.decoder.type == "BP_FLOODING")
	{
		if (params.decoder.implem == "MSO")
			decoder = new Decoder_LDPC_BP_flooding_min_sum_offset<B,R>(params.code.K,
			                                                           params.code.N,
			                                                           params.decoder.n_ite,
			                                                           alist_data,
			                                                           params.decoder.enable_syndrome,
			                                                           params.simulation.inter_frame_level);
		else if (params.decoder.implem == "SPA")
			decoder = new Decoder_LDPC_BP_flooding_sum_product<B,R>(params.code.K,
			                                                        params.code.N,
			                                                        params.decoder.n_ite,
			                                                        alist_data,
			                                                        params.decoder.enable_syndrome,
			                                                        params.simulation.inter_frame_level);
		else if (params.decoder.implem == "LSPA")
			decoder = new Decoder_LDPC_BP_flooding_log_sum_product<B,R>(params.code.K,
			                                                            params.code.N,
			                                                            params.decoder.n_ite,
			                                                            alist_data,
			                                                            params.decoder.enable_syndrome,
			                                                            params.simulation.inter_frame_level);
	}
	else if (params.decoder.type == "BP_LAYERED")
	{
		if (params.decoder.implem == "SPA")
			decoder = new Decoder_LDPC_BP_layered_sum_product<B,R>(params.code.K,
			                                                       params.code.N,
			                                                       params.decoder.n_ite,
			                                                       alist_data,
			                                                       params.decoder.enable_syndrome,
			                                                       params.simulation.inter_frame_level);
		else if (params.decoder.implem == "LSPA")
			decoder = new Decoder_LDPC_BP_layered_log_sum_product<B,R>(params.code.K,
			                                                           params.code.N,
			                                                           params.decoder.n_ite,
			                                                           alist_data,
			                                                           params.decoder.enable_syndrome,
			                                                           params.simulation.inter_frame_level);
	}

	return decoder;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct Factory_decoder_LDPC<B_8,Q_8>;
template struct Factory_decoder_LDPC<B_16,Q_16>;
template struct Factory_decoder_LDPC<B_32,Q_32>;
template struct Factory_decoder_LDPC<B_64,Q_64>;
#else
template struct Factory_decoder_LDPC<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
