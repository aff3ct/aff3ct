#include <string>

#include "Tools/Perf/MIPP/mipp.h"

#include "Module/CRC/NO/CRC_NO.hpp"
#include "Module/CRC/Polynomial/CRC_polynomial.hpp"
#include "Module/CRC/Polynomial/CRC_polynomial_inter.hpp"
#include "Module/CRC/Polynomial/CRC_polynomial_double.hpp"

#include "Factory_CRC.hpp"

template <typename B>
CRC<B>* Factory_CRC<B>
::build(const parameters &params)
{
	CRC<B> *crc = nullptr;

	// build the crc
	if(params.decoder.simd_strategy == "INTER")
	{
		if (!params.crc.type.empty() && params.decoder.type.find("LTE") != std::string::npos)
			crc = new CRC_polynomial_inter<B>(params.code.K, params.crc.type, mipp::nElmtsPerRegister<B>());
		else if (!params.crc.type.empty())
			crc = new CRC_polynomial<B>(params.code.K, params.crc.type);
		else
			crc = new CRC_NO<B>(params.code.K);
	}
	else
	{
		if (!params.crc.type.empty())
			crc = new CRC_polynomial<B>(params.code.K, params.crc.type);
		else
			crc = new CRC_NO<B>(params.code.K);
	}

	return crc;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct Factory_CRC<B_8>;
template struct Factory_CRC<B_16>;
template struct Factory_CRC<B_32>;
template struct Factory_CRC<B_64>;
#else
template struct Factory_CRC<B>;
#endif
// ==================================================================================== explicit template instantiation
