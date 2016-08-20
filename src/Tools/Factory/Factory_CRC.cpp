#include <string>

#include "../../CRC/CRC_NO.hpp"
#include "../../CRC/CRC_polynomial.hpp"
#include "../../CRC/CRC_polynomial_inter.hpp"
#include "../../CRC/CRC_polynomial_double.hpp"

#include "../MIPP/mipp.h"

#include "Factory_CRC.hpp"

template <typename B>
CRC<B>* Factory_CRC<B>
::build(const t_code_param &code_params, const t_decoder_param &deco_params)
{
	CRC<B> *crc = nullptr;

	// build the crc
	if(deco_params.simd_strategy == "INTER")
	{
		if (!code_params.crc.empty() && deco_params.algo.find("LTE") != std::string::npos)
			crc = new CRC_polynomial_inter<B>(code_params.K, code_params.crc, mipp::nElmtsPerRegister<B>());
		else if (!code_params.crc.empty())
			crc = new CRC_polynomial<B>(code_params.K, code_params.crc);
		else
			crc = new CRC_NO<B>(code_params.K);
	}
	else
	{
		if (!code_params.crc.empty())
			crc = new CRC_polynomial<B>(code_params.K, code_params.crc);
		else
			crc = new CRC_NO<B>(code_params.K);
	}

	return crc;
}

// ==================================================================================== explicit template instantiation 
#include "../types.h"
#ifdef MULTI_PREC
template struct Factory_CRC<B_8>;
template struct Factory_CRC<B_16>;
template struct Factory_CRC<B_32>;
template struct Factory_CRC<B_64>;
#else
template struct Factory_CRC<B>;
#endif
// ==================================================================================== explicit template instantiation
