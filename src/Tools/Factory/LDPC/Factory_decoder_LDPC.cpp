#include <string>

#include "../../../Decoder/LDPC/BP/Decoder_LDPC_BP_min_sum_naive.hpp"
#include "../../../Decoder/LDPC/BP/Decoder_LDPC_BP_sum_product_naive.hpp"

#include "Factory_decoder_LDPC.hpp"

template <typename B, typename R>
Decoder_SISO<B,R>* Factory_decoder_LDPC<B,R>
::build(const t_code_param                &code_params,
        const t_encoder_param             &enco_params,
        const t_decoder_param             &deco_params,
        const std ::vector<unsigned char> &n_variables_per_parity,
        const std ::vector<unsigned char> &n_parities_per_variable,
        const std ::vector<unsigned int > &transpose,
        const mipp::vector<B            > &X_N,
        const bool                         coset)
{
	Decoder_SISO<B,R> *decoder = nullptr;

	if (deco_params.algo == "BP" || deco_params.algo == "BP_MIN_SUM")
	{
		if (deco_params.implem == "NAIVE")
			decoder = new Decoder_LDPC_BP_min_sum_naive<B,R>(code_params.K, 
			                                                 code_params.N,
			                                                 deco_params.max_iter,
			                                                 n_variables_per_parity,
			                                                 n_parities_per_variable,
			                                                 transpose,
			                                                 X_N,
			                                                 coset);
	}
	else if (deco_params.algo == "BP_SUM_PRODUCT")
	{
		if (deco_params.implem == "NAIVE")
			decoder = new Decoder_LDPC_BP_sum_product_naive<B,R>(code_params.K,
			                                                     code_params.N,
			                                                     deco_params.max_iter,
			                                                     n_variables_per_parity,
			                                                     n_parities_per_variable,
			                                                     transpose,
			                                                     X_N,
			                                                     coset);
	}

	return decoder;
}

// ==================================================================================== explicit template instantiation 
#include "../../types.h"
#ifdef MULTI_PREC
template struct Factory_decoder_LDPC<B_8,Q_8>;
template struct Factory_decoder_LDPC<B_16,Q_16>;
template struct Factory_decoder_LDPC<B_32,Q_32>;
template struct Factory_decoder_LDPC<B_64,Q_64>;
#else
template struct Factory_decoder_LDPC<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
