#include <utility>

#include "Tools/Documentation/documentation.h"
#include "Module/Coset/Bit/Coset_bit.hpp"
#include "Module/Coset/Real/Coset_real.hpp"
#include "Factory/Module/Coset/Coset.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Coset_name   = "Coset";
const std::string aff3ct::factory::Coset_prefix = "cst";

Coset
::Coset(const std::string &prefix)
: Factory(Coset_name, Coset_name, prefix)
{
}

Coset* Coset
::clone() const
{
	return new Coset(*this);
}

void Coset
::get_description(cli::Argument_map_info &args) const
{
	auto p = this->get_prefix();
	const std::string class_name = "factory::Coset::";

	tools::add_arg(args, p, class_name+"p+size",
		cli::Integer(cli::Positive(), cli::Non_zero()),
		cli::arg_rank::REQ);

	tools::add_arg(args, p, class_name+"p+type",
		cli::Text(cli::Including_set("STD")));
}

void Coset
::store(const cli::Argument_map_value &vals)
{
	auto p = this->get_prefix();

	if(vals.exist({p+"-size", "N"})) this->size = vals.to_int({p+"-size", "N"});
	if(vals.exist({p+"-type"     })) this->type = vals.at    ({p+"-type"     });
}

void Coset
::get_headers(std::map<std::string,tools::header_list>& headers, const bool full) const
{
	auto p = this->get_prefix();

	headers[p].push_back(std::make_pair("Type", this->type));
	if (full) headers[p].push_back(std::make_pair("Size (N)", std::to_string(this->size)));
}

template <typename B1, typename B2>
module::Coset<B1,B2>* Coset
::build_bit() const
{
	if (type == "STD") return new module::Coset_bit<B1,B2>(size);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename R>
module::Coset<B,R>* Coset
::build_real() const
{
	if (type == "STD") return new module::Coset_real<B,R>(size);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::module::Coset<B_8 ,B_8 >* aff3ct::factory::Coset::build_bit<B_8 ,B_8 >() const;
template aff3ct::module::Coset<B_16,B_16>* aff3ct::factory::Coset::build_bit<B_16,B_16>() const;
template aff3ct::module::Coset<B_32,B_32>* aff3ct::factory::Coset::build_bit<B_32,B_32>() const;
template aff3ct::module::Coset<B_64,B_64>* aff3ct::factory::Coset::build_bit<B_64,B_64>() const;
#else
template aff3ct::module::Coset<B,B>* aff3ct::factory::Coset::build_bit<B,B>() const;
#endif

#ifdef AFF3CT_MULTI_PREC
template aff3ct::module::Coset<B_8 ,Q_8 >* aff3ct::factory::Coset::build_real<B_8 ,Q_8 >() const;
template aff3ct::module::Coset<B_16,Q_16>* aff3ct::factory::Coset::build_real<B_16,Q_16>() const;
template aff3ct::module::Coset<B_32,Q_32>* aff3ct::factory::Coset::build_real<B_32,Q_32>() const;
template aff3ct::module::Coset<B_64,Q_64>* aff3ct::factory::Coset::build_real<B_64,Q_64>() const;
#else
template aff3ct::module::Coset<B,Q>* aff3ct::factory::Coset::build_real<B,Q>() const;
#endif
// ==================================================================================== explicit template instantiation
