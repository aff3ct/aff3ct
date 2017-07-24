#include "Tools/Exception/exception.hpp"

#include "Module/CRC/NO/CRC_NO.hpp"
#include "Module/CRC/Polynomial/CRC_polynomial.hpp"
#include "Module/CRC/Polynomial/CRC_polynomial_fast.hpp"
#include "Module/CRC/Polynomial/CRC_polynomial_inter.hpp"

#include "CRC.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::CRC::name   = "CRC";
const std::string aff3ct::factory::CRC::prefix = "crc";

template <typename B>
module::CRC<B>* CRC
::build(const parameters &params)
{
	     if (params.type == "STD"  ) return new module::CRC_polynomial      <B>(params.K, params.poly, params.size, params.n_frames);
	else if (params.type == "FAST" ) return new module::CRC_polynomial_fast <B>(params.K, params.poly, params.size, params.n_frames);
	else if (params.type == "INTER") return new module::CRC_polynomial_inter<B>(params.K, params.poly, params.size, params.n_frames);
	else if (params.type == "NO"   ) return new module::CRC_NO              <B>(params.K,                           params.n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

void CRC
::build_args(arg_map &req_args, arg_map &opt_args, const std::string p)
{
	req_args[{p+"-info-bits", "K"}] =
		{"positive_int",
		 "number of generated bits (information bits, the CRC is not included)."};

	opt_args[{p+"-fra", "F"}] =
		{"positive_int",
		 "set the number of inter frame level to process."};

	opt_args[{p+"-type"}] =
		{"string",
		 "select the CRC implementation you want to use.",
		 "NO, STD, FAST, INTER"};

	opt_args[{p+"-poly"}] =
		{"string",
		 "select the CRC polynomial you want to use (ex: \"8-DVB-S2\": 0xD5, \"16-IBM\": 0x8005, \"24-LTEA\": 0x864CFB, \"32-GZIP\": 0x04C11DB7)."};

	opt_args[{p+"-size"}] =
		{"positive_int",
		 "size of the CRC (divisor size in bit -1), required if you selected an unknown CRC."};
}

void CRC
::store_args(const arg_val_map &vals, parameters &params, const std::string p)
{
	if(exist(vals, {p+"-info-bits", "K"})) params.K        = std::stoi(vals.at({p+"-info-bits", "K"}));
	if(exist(vals, {p+"-fra",       "F"})) params.n_frames = std::stoi(vals.at({p+"-fra",       "F"}));
	if(exist(vals, {p+"-type"          })) params.type     =           vals.at({p+"-type"          });
	if(exist(vals, {p+"-poly"          })) params.poly     =           vals.at({p+"-poly"          });
	if(exist(vals, {p+"-size"          })) params.size     = std::stoi(vals.at({p+"-size"          }));

	if (!params.poly.empty() && !params.size)
		params.size = module::CRC_polynomial<>::size(params.poly);

	if (params.poly.empty())
		params.type = "NO";
}

void CRC
::header(params_list& head_crc, const parameters& params)
{
	if (!params.poly.empty())
	{
		head_crc.push_back(std::make_pair("Type", params.type));
		head_crc.push_back(std::make_pair("Info. bits (K)", std::to_string(params.K)));
		head_crc.push_back(std::make_pair("Inter frame level", std::to_string(params.n_frames)));

		auto poly_name = module::CRC_polynomial<>::name(params.poly);
		if (!poly_name.empty())
			head_crc.push_back(std::make_pair("Name", poly_name));

		std::stringstream poly_val;
		poly_val << "0x" << std::hex << module::CRC_polynomial<>::value(params.poly);
		head_crc.push_back(std::make_pair("Polynomial (hexadecimal)", poly_val.str()));

		auto poly_size = module::CRC_polynomial<>::size (params.poly);
		head_crc.push_back(std::make_pair("Size (in bit)", std::to_string(poly_size ? poly_size : params.size)));
	}
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::CRC<B_8 >* aff3ct::factory::CRC::build<B_8 >(const aff3ct::factory::CRC::parameters&);
template aff3ct::module::CRC<B_16>* aff3ct::factory::CRC::build<B_16>(const aff3ct::factory::CRC::parameters&);
template aff3ct::module::CRC<B_32>* aff3ct::factory::CRC::build<B_32>(const aff3ct::factory::CRC::parameters&);
template aff3ct::module::CRC<B_64>* aff3ct::factory::CRC::build<B_64>(const aff3ct::factory::CRC::parameters&);
#else
template aff3ct::module::CRC<B>* aff3ct::factory::CRC::build<B>(const aff3ct::factory::CRC::parameters&);
#endif
// ==================================================================================== explicit template instantiation
