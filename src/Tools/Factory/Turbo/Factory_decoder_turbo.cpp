#include <string>

#include "Module/Decoder/Turbo/Decoder_turbo_naive.hpp"
#include "Module/Decoder/Turbo/Decoder_turbo_naive.hpp"
#include "Module/Decoder/Turbo/CRC/Decoder_turbo_naive_CA.hpp"
#include "Module/Decoder/Turbo/CRC/Decoder_turbo_fast_CA.hpp"

#include "Factory_decoder_turbo.hpp"

template <typename B, typename R>
Decoder<B,R>* Factory_decoder_turbo<B,R>
::build(const t_code_param        &code_params,
        const t_encoder_param     &enco_params,
        const t_channel_param     &chan_params,
        const t_decoder_param     &deco_params,
        const Interleaver<short>  *interleaver,
              SISO<R>             *siso_n, 
              SISO<R>             *siso_i,
              Scaling_factor<R>   *scaling_factor,
              CRC<B>              *crc)
{
	Decoder<B,R> *decoder = nullptr;

	if (chan_params.domain == "LLR")
	{
		if (typeid(B) == typeid(long long))
		{
			// there is a CRC
			if (crc != nullptr && !code_params.crc.empty())
				decoder = new Decoder_turbo_naive_CA<B,R>(code_params.K, code_params.N, deco_params.max_iter, 
				                                          *interleaver, *siso_n, *siso_i, *scaling_factor, *crc, 
				                                          enco_params.buffered);
			// there is no CRC
			else
				decoder = new Decoder_turbo_naive<B,R>(code_params.K, code_params.N, deco_params.max_iter,
				                                       *interleaver, *siso_n, *siso_i, *scaling_factor, 
				                                       enco_params.buffered);
		}
		else
		{
			// there is a CRC
			if (crc != nullptr && !code_params.crc.empty())
				decoder = new Decoder_turbo_fast_CA<B,R>(code_params.K, code_params.N, deco_params.max_iter, 
				                                         *interleaver, *siso_n, *siso_i, *scaling_factor, *crc, 
				                                         enco_params.buffered);
			// there is no CRC
			else
				decoder = new Decoder_turbo_fast<B,R>(code_params.K, code_params.N, deco_params.max_iter,
				                                      *interleaver, *siso_n, *siso_i, *scaling_factor, 
				                                      enco_params.buffered);
		}
	}

	return decoder;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct Factory_decoder_turbo<B_8,Q_8>;
template struct Factory_decoder_turbo<B_16,Q_16>;
template struct Factory_decoder_turbo<B_32,Q_32>;
template struct Factory_decoder_turbo<B_64,Q_64>;
#else
template struct Factory_decoder_turbo<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
