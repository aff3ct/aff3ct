#include <string>

#include "Module/Decoder/Turbo/Decoder_turbo_naive.hpp"
#include "Module/Decoder/Turbo/Decoder_turbo_naive.hpp"
#include "Module/Decoder/Turbo/CRC/Decoder_turbo_naive_CA.hpp"
#include "Module/Decoder/Turbo/CRC/Self_corrected/Decoder_turbo_naive_CA_self_corrected.hpp"
#include "Module/Decoder/Turbo/CRC/Decoder_turbo_fast_CA.hpp"

#include "Factory_decoder_turbo.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B, typename R>
Decoder<B,R>* Factory_decoder_turbo<B,R>
::build(const parameters          &params,
        const Interleaver<short>  *interleaver,
              SISO<R>             *siso_n, 
              SISO<R>             *siso_i,
              Scaling_factor<R>   *scaling_factor,
              CRC<B>              *crc)
{
	Decoder<B,R> *decoder = nullptr;

	if (params.channel.domain == "LLR")
	{
		if (typeid(B) == typeid(long long))
		{
			// there is a CRC
			if (crc != nullptr && !params.crc.type.empty())
			{
				if(!params.decoder.self_corrected)
					decoder = new Decoder_turbo_naive_CA<B,R>(params.code.K, params.code.N, params.decoder.n_ite,
					                                          *interleaver, *siso_n, *siso_i, *scaling_factor, *crc,
					                                          params.encoder.buffered);
				else
					decoder = new Decoder_turbo_naive_CA_self_corrected<B,R>(params.code.K, params.code.N,
					                                                         params.decoder.n_ite,
					                                                         *interleaver, *siso_n, *siso_i,
					                                                         *scaling_factor, *crc,
					                                                         params.encoder.buffered);
			}
			// there is no CRC
			else
				decoder = new Decoder_turbo_naive<B,R>(params.code.K, params.code.N, params.decoder.n_ite,
				                                       *interleaver, *siso_n, *siso_i, *scaling_factor, 
				                                       params.encoder.buffered);
		}
		else
		{
			// there is a CRC
			if (crc != nullptr && !params.crc.type.empty())
			{
				if(!params.decoder.self_corrected)
					decoder = new Decoder_turbo_fast_CA<B,R>(params.code.K, params.code.N, params.decoder.n_ite,
					                                         *interleaver, *siso_n, *siso_i, *scaling_factor, *crc,
					                                         params.encoder.buffered);
				else
					decoder = new Decoder_turbo_naive_CA_self_corrected<B,R>(params.code.K, params.code.N,
					                                                         params.decoder.n_ite, *interleaver,
					                                                         *siso_n, *siso_i, *scaling_factor, *crc,
					                                                         params.encoder.buffered);
			}
			// there is no CRC
			else
				decoder = new Decoder_turbo_fast<B,R>(params.code.K, params.code.N, params.decoder.n_ite,
				                                      *interleaver, *siso_n, *siso_i, *scaling_factor, 
				                                      params.encoder.buffered);
		}
	}

	return decoder;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct aff3ct::tools::Factory_decoder_turbo<B_8,Q_8>;
template struct aff3ct::tools::Factory_decoder_turbo<B_16,Q_16>;
template struct aff3ct::tools::Factory_decoder_turbo<B_32,Q_32>;
template struct aff3ct::tools::Factory_decoder_turbo<B_64,Q_64>;
#else
template struct aff3ct::tools::Factory_decoder_turbo<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
