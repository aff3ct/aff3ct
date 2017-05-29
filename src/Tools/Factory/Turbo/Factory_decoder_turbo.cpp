#include <string>

#include "Module/Decoder/Turbo/Decoder_turbo_naive.hpp"
#include <stdexcept>

#include "Module/Decoder/Turbo/Decoder_turbo_naive.hpp"
#include "Module/Decoder/Turbo/CRC/Decoder_turbo_naive_CA.hpp"
#include "Module/Decoder/Turbo/CRC/Self_corrected/Decoder_turbo_naive_CA_self_corrected.hpp"
#include "Module/Decoder/Turbo/CRC/Flip_and_check/Decoder_turbo_naive_CA_flip_and_check.hpp"
#include "Module/Decoder/Turbo/CRC/Decoder_turbo_fast_CA.hpp"

#include "Factory_decoder_turbo.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B, typename R>
Decoder<B,R>* Factory_decoder_turbo<B,R>
::build(const std::string          type,
        const int                  K,
        const int                  N,
        const int                  n_ite,
        const Interleaver<int>    &itl,
              SISO<R>             &siso_n,
              SISO<R>             &siso_i,
              Scaling_factor<R>   &sf,
              CRC<B>              *crc,
        const int                  fnc_q,
        const int                  fnc_ite_min,
        const int                  fnc_ite_max,
        const int                  fnc_ite_step,
        const bool                 buffered)
{
	const auto _fnc_ite_max = fnc_ite_max == -1 ? n_ite : fnc_ite_max;

	if (crc != nullptr && crc->get_size() > 0)
	{
		     if (type == "TURBO_SC" ) return new Decoder_turbo_naive_CA_self_corrected<B,R>(K, N, n_ite, itl, siso_n, siso_i, sf, *crc,                                                 buffered);
		else if (type == "TURBO_FNC") return new Decoder_turbo_naive_CA_flip_and_check<B,R>(K, N, n_ite, itl, siso_n, siso_i, sf, *crc, fnc_q, fnc_ite_min, _fnc_ite_max, fnc_ite_step, buffered);
		if (typeid(B) == typeid(long long))
		{
		     if (type == "TURBO"    ) return new Decoder_turbo_naive_CA               <B,R>(K, N, n_ite, itl, siso_n, siso_i, sf, *crc,                                                 buffered);
		}
		else
		{
		     if (type == "TURBO"    ) return new Decoder_turbo_fast_CA                <B,R>(K, N, n_ite, itl, siso_n, siso_i, sf, *crc,                                                 buffered);
		}
	}
	else if (typeid(B) == typeid(long long))
	{
		     if (type == "TURBO"    ) return new Decoder_turbo_naive                  <B,R>(K, N, n_ite, itl, siso_n, siso_i, sf,                                                       buffered);
	}
	else
	{
		     if (type == "TURBO"    ) return new Decoder_turbo_fast                   <B,R>(K, N, n_ite, itl, siso_n, siso_i, sf,                                                       buffered);
	}

	throw std::runtime_error("aff3ct::tools::Factory_decoder_turbo: the factory could not allocate the object.");
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
