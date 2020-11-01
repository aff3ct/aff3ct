#include "Tools/Exception/exception.hpp"
#include "Tools/Documentation/documentation.h"
#include "Module/Encoder/Polar/Encoder_polar.hpp"
#include "Module/Encoder/Polar/Encoder_polar_sys.hpp"
#include "Factory/Module/Encoder/Polar/Encoder_polar.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Encoder_polar_name   = "Encoder Polar";
const std::string aff3ct::factory::Encoder_polar_prefix = "enc";

Encoder_polar
::Encoder_polar(const std::string &prefix)
: Encoder(Encoder_polar_name, prefix)
{
	this->type = "POLAR";
}

Encoder_polar* Encoder_polar
::clone() const
{
	return new Encoder_polar(*this);
}

void Encoder_polar
::get_description(cli::Argument_map_info &args) const
{
	Encoder::get_description(args);

	auto p = this->get_prefix();
	const std::string class_name = "factory::Encoder_polar::";

	cli::add_options(args.at({p+"-type"}), 0, "POLAR");

	tools::add_arg(args, p, class_name+"p+no-sys",
		cli::None());
}

void Encoder_polar
::store(const cli::Argument_map_value &vals)
{
	Encoder::store(vals);
}

void Encoder_polar
::get_headers(std::map<std::string,tools::header_list>& headers, const bool full) const
{
	Encoder::get_headers(headers, full);
}

template <typename B>
module::Encoder_polar<B>* Encoder_polar
::build(const std::vector<bool> &frozen_bits) const
{
	if (this->type == "POLAR" && !this->systematic) return new module::Encoder_polar    <B>(this->K, this->N_cw, frozen_bits);
	if (this->type == "POLAR" &&  this->systematic) return new module::Encoder_polar_sys<B>(this->K, this->N_cw, frozen_bits);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::module::Encoder_polar<B_8 >* aff3ct::factory::Encoder_polar::build<B_8 >(const std::vector<bool>&) const;
template aff3ct::module::Encoder_polar<B_16>* aff3ct::factory::Encoder_polar::build<B_16>(const std::vector<bool>&) const;
template aff3ct::module::Encoder_polar<B_32>* aff3ct::factory::Encoder_polar::build<B_32>(const std::vector<bool>&) const;
template aff3ct::module::Encoder_polar<B_64>* aff3ct::factory::Encoder_polar::build<B_64>(const std::vector<bool>&) const;
#else
template aff3ct::module::Encoder_polar<B>* aff3ct::factory::Encoder_polar::build<B>(const std::vector<bool>&) const;
#endif
// ==================================================================================== explicit template instantiation
