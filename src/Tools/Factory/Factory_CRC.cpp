#include "Tools/Exception/exception.hpp"

#include "Module/CRC/NO/CRC_NO.hpp"
#include "Module/CRC/Polynomial/CRC_polynomial.hpp"
#include "Module/CRC/Polynomial/CRC_polynomial_fast.hpp"
#include "Module/CRC/Polynomial/CRC_polynomial_inter.hpp"

#include "Factory_CRC.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B>
CRC<B>* Factory_CRC<B>
::build(const std::string type,
        const int         K,
        const int         size,
        const std::string poly,
        const int         n_frames)
{
	     if (type == "STD"  ) return new CRC_polynomial      <B>(K, poly, size, n_frames);
	else if (type == "FAST" ) return new CRC_polynomial_fast <B>(K, poly, size, n_frames);
	else if (type == "INTER") return new CRC_polynomial_inter<B>(K, poly, size, n_frames);
	else if (type == "NO"   ) return new CRC_NO              <B>(K,             n_frames);

	throw cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B>
void Factory_CRC<B>
::build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args)
{
	// ----------------------------------------------------------------------------------------------------------- crc
	opt_args[{"crc-type"}] =
		{"string",
		 "select the CRC implementation you want to use.",
		 "STD, FAST"};

	opt_args[{"crc-poly"}] =
		{"string",
		 "select the CRC polynomial you want to use (ex: \"8-DVB-S2\": 0xD5, \"16-IBM\": 0x8005, \"24-LTEA\": 0x864CFB, \"32-GZIP\": 0x04C11DB7)."};

	opt_args[{"crc-size"}] =
		{"positive_int",
		 "size of the CRC (divisor size in bit -1), required if you selected an unknown CRC."};

	opt_args[{"crc-rate"}] =
		{"",
		 "enable the CRC to be counted in the code rate computation."};
}

template <typename B>
void Factory_CRC<B>
::store_args(const Arguments_reader& ar, CRC_params &params, const int K, const int N)
{
	// ----------------------------------------------------------------------------------------------------------- crc
	if(ar.exist_arg({"crc-type"})) params.type = ar.get_arg    ({"crc-type"});
	if(ar.exist_arg({"crc-poly"})) params.poly = ar.get_arg    ({"crc-poly"});
	if(ar.exist_arg({"crc-size"})) params.size = ar.get_arg_int({"crc-size"});
	if(ar.exist_arg({"crc-rate"})) params.inc_code_rate = true;

	if (!params.poly.empty() && !params.size)
		params.size = CRC_polynomial<B>::size(params.poly);

	// update the code rate R and K_info
	auto real_K = K;
	if (!params.poly.empty() && !params.inc_code_rate)
		real_K -= params.size;
	params.R      = real_K / (float)N;
	params.K_info = real_K;
}

template <typename B>
void Factory_CRC<B>
::group_args(Arguments_reader::arg_grp& ar)
{
	ar.push_back({"crc", "CRC parameter(s)"});
}


// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct aff3ct::tools::Factory_CRC<B_8>;
template struct aff3ct::tools::Factory_CRC<B_16>;
template struct aff3ct::tools::Factory_CRC<B_32>;
template struct aff3ct::tools::Factory_CRC<B_64>;
#else
template struct aff3ct::tools::Factory_CRC<B>;
#endif
// ==================================================================================== explicit template instantiation
