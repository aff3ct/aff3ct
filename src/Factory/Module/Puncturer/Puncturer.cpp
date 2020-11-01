#include <utility>

#include "Tools/Documentation/documentation.h"
#include "Module/Puncturer/NO/Puncturer_NO.hpp"
#include "Factory/Module/Puncturer/Puncturer.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Puncturer_name   = "Puncturer";
const std::string aff3ct::factory::Puncturer_prefix = "pct";

Puncturer
::Puncturer(const std::string &prefix)
: Factory(Puncturer_name, Puncturer_name, prefix)
{
}

Puncturer
::Puncturer(const std::string &name, const std::string &prefix)
: Factory(name, Puncturer_name, prefix)
{
}

Puncturer* Puncturer
::clone() const
{
	return new Puncturer(*this);
}

void Puncturer
::get_description(cli::Argument_map_info &args) const
{
	auto p = this->get_prefix();
	const std::string class_name = "factory::Puncturer::";

	tools::add_arg(args, p, class_name+"p+info-bits,K",
		cli::Integer(cli::Positive(), cli::Non_zero()),
		cli::arg_rank::REQ);

	tools::add_arg(args, p, class_name+"p+fra-size,N",
		cli::Integer(cli::Positive(), cli::Non_zero()),
		cli::arg_rank::REQ);

	tools::add_arg(args, p, class_name+"p+type",
		cli::Text(cli::Including_set("NO")));
}

void Puncturer
::store(const cli::Argument_map_value &vals)
{
	auto p = this->get_prefix();

	if(vals.exist({p+"-info-bits", "K"})) this->K    = vals.to_int({p+"-info-bits", "K"});
	if(vals.exist({p+"-fra-size",  "N"})) this->N    = vals.to_int({p+"-fra-size",  "N"});
	if(vals.exist({p+"-type"          })) this->type = vals.at    ({p+"-type"          });

	this->N_cw = this->N;
}

void Puncturer
::get_headers(std::map<std::string,tools::header_list>& headers, const bool full) const
{
	auto p = this->get_prefix();

	headers[p].push_back(std::make_pair("Type", this->type));
	if (full) headers[p].push_back(std::make_pair("Info. bits (K)", std::to_string(this->K)));
	if (full) headers[p].push_back(std::make_pair("Frame size (N)", std::to_string(this->N)));
	if (full) headers[p].push_back(std::make_pair("Codeword size", std::to_string(this->N_cw)));
}

template <typename B, typename Q>
module::Puncturer<B,Q>* Puncturer
::build() const
{
	if (this->type == "NO") return new module::Puncturer_NO<B,Q>(this->K, this->N);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::module::Puncturer<B_8 ,Q_8 >* aff3ct::factory::Puncturer::build<B_8 ,Q_8 >() const;
template aff3ct::module::Puncturer<B_16,Q_16>* aff3ct::factory::Puncturer::build<B_16,Q_16>() const;
template aff3ct::module::Puncturer<B_32,Q_32>* aff3ct::factory::Puncturer::build<B_32,Q_32>() const;
template aff3ct::module::Puncturer<B_64,Q_64>* aff3ct::factory::Puncturer::build<B_64,Q_64>() const;
#else
template aff3ct::module::Puncturer<B,Q>* aff3ct::factory::Puncturer::build<B,Q>() const;
#endif
// ==================================================================================== explicit template instantiation
