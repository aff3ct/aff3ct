#include <cmath>

#include "Tools/Exception/exception.hpp"
#include "Factory/Module/Puncturer/Polar/Puncturer_polar.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Puncturer_polar_name   = "Puncturer Polar";
const std::string aff3ct::factory::Puncturer_polar_prefix = "pct";

Puncturer_polar
::Puncturer_polar(const std::string &prefix)
: Puncturer(Puncturer_polar_name, prefix)
{
	this->type = "SHORTLAST";
}

Puncturer_polar* Puncturer_polar
::clone() const
{
	return new Puncturer_polar(*this);
}

void Puncturer_polar
::get_description(cli::Argument_map_info &args) const
{
	Puncturer::get_description(args);

	auto p = this->get_prefix();

	cli::add_options(args.at({p+"-type"}), 0, "SHORTLAST");
}

void Puncturer_polar
::store(const cli::Argument_map_value &vals)
{
	Puncturer::store(vals);
	this->N_cw = (int)std::exp2((int)std::ceil(std::log2(this->N)));

	if (this->N == this->N_cw)
		this->type = "NO";
}

void Puncturer_polar
::get_headers(std::map<std::string,tools::header_list>& headers, const bool full) const
{
	Puncturer::get_headers(headers, full);
}

template <typename B, typename Q>
module::Puncturer_polar_shortlast<B,Q>* Puncturer_polar
::build(const tools::Frozenbits_generator &fb_generator) const
{
	if (this->type == "SHORTLAST") return new module::Puncturer_polar_shortlast<B,Q>(this->K, this->N, fb_generator);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::module::Puncturer_polar_shortlast<B_8 ,Q_8 >* aff3ct::factory::Puncturer_polar::build<B_8 ,Q_8 >(const aff3ct::tools::Frozenbits_generator&) const;
template aff3ct::module::Puncturer_polar_shortlast<B_16,Q_16>* aff3ct::factory::Puncturer_polar::build<B_16,Q_16>(const aff3ct::tools::Frozenbits_generator&) const;
template aff3ct::module::Puncturer_polar_shortlast<B_32,Q_32>* aff3ct::factory::Puncturer_polar::build<B_32,Q_32>(const aff3ct::tools::Frozenbits_generator&) const;
template aff3ct::module::Puncturer_polar_shortlast<B_64,Q_64>* aff3ct::factory::Puncturer_polar::build<B_64,Q_64>(const aff3ct::tools::Frozenbits_generator&) const;
#else
template aff3ct::module::Puncturer_polar_shortlast<B,Q>* aff3ct::factory::Puncturer_polar::build<B,Q>(const aff3ct::tools::Frozenbits_generator&) const;
#endif
// ==================================================================================== explicit template instantiation
