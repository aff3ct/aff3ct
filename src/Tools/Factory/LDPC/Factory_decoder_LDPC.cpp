#include <string>

#include "Module/Decoder/LDPC/BP/Decoder_LDPC_BP_flooding_min_sum.hpp"
#include "Module/Decoder/LDPC/BP/Decoder_LDPC_BP_flooding_sum_product.hpp"

#include "Factory_decoder_LDPC.hpp"

template <typename B, typename R>
Decoder_SISO<B,R>* Factory_decoder_LDPC<B,R>
::build(const parameters                  &params,
        const std ::vector<unsigned char> &n_variables_per_parity,
        const std ::vector<unsigned char> &n_parities_per_variable,
        const std ::vector<unsigned int > &transpose,
        const mipp::vector<B            > &X_N)
{
	Decoder_SISO<B,R> *decoder = nullptr;

	if (params.decoder.algo == "BP" || params.decoder.algo == "BP_FLOODING")
	{
		if (params.decoder.implem == "MIN_SUM")
			decoder = new Decoder_LDPC_BP_flooding_min_sum<B,R>(params.code.K,
			                                                    params.code.N,
			                                                    params.decoder.max_iter,
			                                                    n_variables_per_parity,
			                                                    n_parities_per_variable,
			                                                    transpose,
			                                                    X_N,
			                                                    params.code.coset);
		else if (params.decoder.implem == "SUM_PRODUCT")
			decoder = new Decoder_LDPC_BP_flooding_sum_product<B,R>(params.code.K,
			                                                        params.code.N,
			                                                        params.decoder.max_iter,
			                                                        n_variables_per_parity,
			                                                        n_parities_per_variable,
			                                                        transpose,
			                                                        X_N,
			                                                        params.code.coset);
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
