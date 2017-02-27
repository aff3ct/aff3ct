#include <string>

#include "Module/Decoder/LDPC/BP/Flooding/SPA/Decoder_LDPC_BP_flooding_sum_product.hpp"
#include "Module/Decoder/LDPC/BP/Flooding/LSPA/Decoder_LDPC_BP_flooding_log_sum_product.hpp"
#include "Module/Decoder/LDPC/BP/Flooding/ONMS/Decoder_LDPC_BP_flooding_offset_normalize_min_sum.hpp"
#include "Module/Decoder/LDPC/BP/Flooding/Gallager/Decoder_LDPC_BP_flooding_Gallager_A.hpp"
#include "Module/Decoder/LDPC/BP/Layered/SPA/Decoder_LDPC_BP_layered_sum_product.hpp"
#include "Module/Decoder/LDPC/BP/Layered/LSPA/Decoder_LDPC_BP_layered_log_sum_product.hpp"
#include "Module/Decoder/LDPC/BP/Layered/ONMS/Decoder_LDPC_BP_layered_offset_normalize_min_sum.hpp"

#include "Factory_decoder_LDPC.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B, typename R>
Decoder_SISO<B,R>* Factory_decoder_LDPC<B,R>
::build(const parameters &params, const AList_reader &alist_data)
{
	Decoder_SISO<B,R> *decoder = nullptr;

	if (params.decoder.type == "BP" || params.decoder.type == "BP_FLOODING")
	{
		if (params.decoder.implem == "ONMS")
			decoder = new Decoder_LDPC_BP_flooding_offset_normalize_min_sum<B,R>(params.code.K,
			                                                                     params.code.N,
			                                                                     params.decoder.n_ite,
			                                                                     alist_data,
			                                                                     params.decoder.normalize_factor,
			                                                                     params.decoder.offset,
			                                                                     params.decoder.enable_syndrome,
			                                                                     params.simulation.inter_frame_level);
		else if (params.decoder.implem == "GALA")
			decoder = new Decoder_LDPC_BP_flooding_Gallager_A<B,R>(params.code.K,
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
		if (params.decoder.implem == "ONMS")
			decoder = new Decoder_LDPC_BP_layered_offset_normalize_min_sum<B,R>(params.code.K,
			                                                                    params.code.N,
			                                                                    params.decoder.n_ite,
			                                                                    alist_data,
			                                                                    params.decoder.normalize_factor,
			                                                                    params.decoder.offset,
			                                                                    params.decoder.enable_syndrome,
			                                                                    params.simulation.inter_frame_level);
		else if (params.decoder.implem == "SPA")
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
template struct aff3ct::tools::Factory_decoder_LDPC<B_8,Q_8>;
template struct aff3ct::tools::Factory_decoder_LDPC<B_16,Q_16>;
template struct aff3ct::tools::Factory_decoder_LDPC<B_32,Q_32>;
template struct aff3ct::tools::Factory_decoder_LDPC<B_64,Q_64>;
#else
template struct aff3ct::tools::Factory_decoder_LDPC<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
