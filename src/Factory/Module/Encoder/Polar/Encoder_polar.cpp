#include "Tools/Exception/exception.hpp"

#include "Module/Encoder/Polar/Encoder_polar.hpp"
#include "Module/Encoder/Polar/Encoder_polar_sys.hpp"

#include "Encoder_polar.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Encoder_polar_name   = "Encoder Polar";
const std::string aff3ct::factory::Encoder_polar_prefix = "enc";

Encoder_polar::parameters
::parameters(const std::string &prefix)
: Encoder::parameters(Encoder_polar_name, prefix)
{
	this->type = "POLAR";
}

Encoder_polar::parameters
::~parameters()
{
}

Encoder_polar::parameters* Encoder_polar::parameters
::clone() const
{
	return new Encoder_polar::parameters(*this);
}

void Encoder_polar::parameters
::get_description(tools::Argument_map_info &req_args, tools::Argument_map_info &opt_args) const
{
	Encoder::parameters::get_description(req_args, opt_args);

	auto p = this->get_prefix();

	tools::add_options(opt_args.at({p+"-type"}), 0, "POLAR");

	opt_args.add(
		{p+"-no-sys"},
		tools::None(),
		"disable the systematic encoding.");
}

void Encoder_polar::parameters
::store(const tools::Argument_map_value &vals)
{
	Encoder::parameters::store(vals);
}

void Encoder_polar::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	Encoder::parameters::get_headers(headers, full);
}

template <typename B>
module::Encoder<B>* Encoder_polar::parameters
::build(const std::vector<bool> &frozen_bits) const
{
	     if (this->type == "POLAR" && !this->systematic) return new module::Encoder_polar    <B>(this->K, this->N_cw, frozen_bits, this->n_frames);
	else if (this->type == "POLAR" &&  this->systematic) return new module::Encoder_polar_sys<B>(this->K, this->N_cw, frozen_bits, this->n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B>
module::Encoder<B>* Encoder_polar
::build(const parameters& params, const std::vector<bool> &frozen_bits)
{
	return params.template build<B>(frozen_bits);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Encoder<B_8 >* aff3ct::factory::Encoder_polar::parameters::build<B_8 >(const std::vector<bool>&) const;
template aff3ct::module::Encoder<B_16>* aff3ct::factory::Encoder_polar::parameters::build<B_16>(const std::vector<bool>&) const;
template aff3ct::module::Encoder<B_32>* aff3ct::factory::Encoder_polar::parameters::build<B_32>(const std::vector<bool>&) const;
template aff3ct::module::Encoder<B_64>* aff3ct::factory::Encoder_polar::parameters::build<B_64>(const std::vector<bool>&) const;
template aff3ct::module::Encoder<B_8 >* aff3ct::factory::Encoder_polar::build<B_8 >(const aff3ct::factory::Encoder_polar::parameters&, const std::vector<bool>&);
template aff3ct::module::Encoder<B_16>* aff3ct::factory::Encoder_polar::build<B_16>(const aff3ct::factory::Encoder_polar::parameters&, const std::vector<bool>&);
template aff3ct::module::Encoder<B_32>* aff3ct::factory::Encoder_polar::build<B_32>(const aff3ct::factory::Encoder_polar::parameters&, const std::vector<bool>&);
template aff3ct::module::Encoder<B_64>* aff3ct::factory::Encoder_polar::build<B_64>(const aff3ct::factory::Encoder_polar::parameters&, const std::vector<bool>&);
#else
template aff3ct::module::Encoder<B>* aff3ct::factory::Encoder_polar::parameters::build<B>(const std::vector<bool>&) const;
template aff3ct::module::Encoder<B>* aff3ct::factory::Encoder_polar::build<B>(const aff3ct::factory::Encoder_polar::parameters&, const std::vector<bool>&);
#endif
// ==================================================================================== explicit template instantiation
