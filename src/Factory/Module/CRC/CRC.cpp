#include <utility>
#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Documentation/documentation.h"
#include "Tools/types.h"
#include "Module/CRC/NO/CRC_NO.hpp"
#include "Module/CRC/Polynomial/CRC_polynomial.hpp"
#include "Module/CRC/Polynomial/CRC_polynomial_fast.hpp"
#include "Module/CRC/Polynomial/CRC_polynomial_inter.hpp"
#include "Factory/Module/CRC/CRC.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::CRC_name   = "CRC";
const std::string aff3ct::factory::CRC_prefix = "crc";

CRC
::CRC(const std::string &prefix)
: Factory(CRC_name, CRC_name, prefix)
{
}

CRC* CRC
::clone() const
{
	return new CRC(*this);
}

void CRC
::get_description(cli::Argument_map_info &args) const
{
	auto p = this->get_prefix();
	const std::string class_name = "factory::CRC::";

	tools::add_arg(args, p, class_name+"p+info-bits,K",
		cli::Integer(cli::Positive(), cli::Non_zero()),
		cli::arg_rank::REQ);

	tools::add_arg(args, p, class_name+"p+fra,F",
		cli::Integer(cli::Positive(), cli::Non_zero()));

	tools::add_arg(args, p, class_name+"p+type,p+poly",
		cli::Text());

	tools::add_arg(args, p, class_name+"p+implem",
		cli::Text(cli::Including_set("STD", "FAST", "INTER")));

	tools::add_arg(args, p, class_name+"p+size",
		cli::Integer(cli::Positive()));
}

void CRC
::store(const cli::Argument_map_value &vals)
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

void CRC
::get_headers(std::map<std::string,tools::header_list>& headers, const bool full) const
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
module::CRC<B>* CRC
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

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::module::CRC<B_8 >* aff3ct::factory::CRC::build<B_8 >() const;
template aff3ct::module::CRC<B_16>* aff3ct::factory::CRC::build<B_16>() const;
template aff3ct::module::CRC<B_32>* aff3ct::factory::CRC::build<B_32>() const;
template aff3ct::module::CRC<B_64>* aff3ct::factory::CRC::build<B_64>() const;
#else
template aff3ct::module::CRC<B>* aff3ct::factory::CRC::build<B>() const;
#endif
// ==================================================================================== explicit template instantiation
