#include "Tools/Exception/exception.hpp"
#include "Tools/types.h"

#include "Module/CRC/NO/CRC_NO.hpp"
#include "Module/CRC/Polynomial/CRC_polynomial.hpp"
#include "Module/CRC/Polynomial/CRC_polynomial_fast.hpp"
#include "Module/CRC/Polynomial/CRC_polynomial_inter.hpp"

#include "CRC.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::CRC_name   = "CRC";
const std::string aff3ct::factory::CRC_prefix = "crc";

CRC::parameters
::parameters(const std::string &prefix)
: Factory::parameters(CRC_name, CRC_name, prefix)
{
}

CRC::parameters* CRC::parameters
::clone() const
{
	return new CRC::parameters(*this);
}

void CRC::parameters
::get_description(tools::Argument_map_info &args) const
{
	auto p = this->get_prefix();

	args.add(
		{p+"-info-bits", "K"},
		tools::Integer(tools::Positive(), tools::Non_zero()),
		"number of generated bits (information bits, the CRC is not included).",
		tools::arg_rank::REQ);

	args.add(
		{p+"-fra", "F"},
		tools::Integer(tools::Positive(), tools::Non_zero()),
		"set the number of inter frame level to process.");

	args.add(
		{p+"-type", p+"-poly"},
		tools::Text(),
		"select the CRC type/polynomial you want to use (ex: \"8-DVB-S2\": 0xD5, \"16-IBM\": 0x8005, \"24-LTEA\": 0x864CFB, \"32-GZIP\": 0x04C11DB7).");

	args.add(
		{p+"-implem"},
		tools::Text(tools::Including_set("STD", "FAST", "INTER")),
		"select the CRC implementation you want to use.");

	args.add(
		{p+"-size"},
		tools::Integer(tools::Positive()),
		"size of the CRC (divisor size in bit -1), required if you selected an unknown CRC.");
}

void CRC::parameters
::store(const tools::Argument_map_value &vals)
{
	auto p = this->get_prefix();

	if(vals.exist({p+"-info-bits",  "K"})) this->K        = vals.to_int({p+"-info-bits",  "K"});
	if(vals.exist({p+"-fra",        "F"})) this->n_frames = vals.to_int({p+"-fra",        "F"});
	if(vals.exist({p+"-type", p+"-poly"})) this->type     = vals.at    ({p+"-type", p+"-poly"});
	if(vals.exist({p+"-implem"         })) this->implem   = vals.at    ({p+"-implem"         });
	if(vals.exist({p+"-size"           })) this->size     = vals.to_int({p+"-size"           });

	if (this->type != "NO" && !this->type.empty() && !this->size)
		this->size = module::CRC_polynomial<B>::get_size(this->type);
}

void CRC::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	auto p = this->get_prefix();

	if (this->type != "NO" && !this->type.empty())
	{
		auto poly_name = module::CRC_polynomial<B>::get_name(this->type);
		if (!poly_name.empty())
			headers[p].push_back(std::make_pair("Type", poly_name));
		else
		{
			std::stringstream poly_val;
			poly_val << "0x" << std::hex << module::CRC_polynomial<B>::get_value(this->type);
			headers[p].push_back(std::make_pair("Type", poly_val.str()));
		}
		std::stringstream poly_val;
		poly_val << "0x" << std::hex << module::CRC_polynomial<B>::get_value(this->type);
		headers[p].push_back(std::make_pair("Polynomial (hexadecimal)", poly_val.str()));

		auto poly_size = module::CRC_polynomial<B>::get_size(this->type);
		headers[p].push_back(std::make_pair("Size (in bit)", std::to_string(poly_size ? poly_size : this->size)));
	}
	else
		headers[p].push_back(std::make_pair("Type", "NO"));

	headers[p].push_back(std::make_pair("Implementation", this->implem));

	if (full) headers[p].push_back(std::make_pair("Info. bits (K)", std::to_string(this->K)));
	if (full) headers[p].push_back(std::make_pair("Inter frame level", std::to_string(this->n_frames)));
}

template <typename B>
module::CRC<B>* CRC::parameters
::build() const
{
	if (this->type != "NO" && !this->type.empty())
	{
		const auto poly = this->type;

		if (this->implem == "STD"  ) return new module::CRC_polynomial      <B>(K, poly, size, n_frames);
		if (this->implem == "FAST" ) return new module::CRC_polynomial_fast <B>(K, poly, size, n_frames);
		if (this->implem == "INTER") return new module::CRC_polynomial_inter<B>(K, poly, size, n_frames);
	}
	else                             return new module::CRC_NO              <B>(K,             n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B>
module::CRC<B>* CRC
::build(const parameters &params)
{
	return params.template build<B>();
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::CRC<B_8 >* aff3ct::factory::CRC::parameters::build<B_8 >() const;
template aff3ct::module::CRC<B_16>* aff3ct::factory::CRC::parameters::build<B_16>() const;
template aff3ct::module::CRC<B_32>* aff3ct::factory::CRC::parameters::build<B_32>() const;
template aff3ct::module::CRC<B_64>* aff3ct::factory::CRC::parameters::build<B_64>() const;
template aff3ct::module::CRC<B_8 >* aff3ct::factory::CRC::build<B_8 >(const aff3ct::factory::CRC::parameters&);
template aff3ct::module::CRC<B_16>* aff3ct::factory::CRC::build<B_16>(const aff3ct::factory::CRC::parameters&);
template aff3ct::module::CRC<B_32>* aff3ct::factory::CRC::build<B_32>(const aff3ct::factory::CRC::parameters&);
template aff3ct::module::CRC<B_64>* aff3ct::factory::CRC::build<B_64>(const aff3ct::factory::CRC::parameters&);
#else
template aff3ct::module::CRC<B>* aff3ct::factory::CRC::parameters::build<B>() const;
template aff3ct::module::CRC<B>* aff3ct::factory::CRC::build<B>(const aff3ct::factory::CRC::parameters&);
#endif
// ==================================================================================== explicit template instantiation
