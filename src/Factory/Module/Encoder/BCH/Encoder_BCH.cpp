#include <cmath>

#include "Tools/Exception/exception.hpp"
#include "Tools/Math/utils.h"

#include "Module/Encoder/BCH/Encoder_BCH.hpp"

#include "Encoder_BCH.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Encoder_BCH_name   = "Encoder BCH";
const std::string aff3ct::factory::Encoder_BCH_prefix = "enc";

Encoder_BCH::parameters
::parameters(const std::string &prefix)
: Encoder::parameters(Encoder_BCH_name, prefix)
{
	this->type = "BCH";
}

Encoder_BCH::parameters
::~parameters()
{
}

Encoder_BCH::parameters* Encoder_BCH::parameters
::clone() const
{
	return new Encoder_BCH::parameters(*this);
}

void Encoder_BCH::parameters
::get_description(tools::Argument_map_info &args) const
{
	Encoder::parameters::get_description(args);

	auto p = this->get_prefix();

	tools::add_options(args.at({p+"-type"}), 0, "BCH");
}

void Encoder_BCH::parameters
::store(const tools::Argument_map_value &vals)
{
	Encoder::parameters::store(vals);
}

void Encoder_BCH::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	Encoder::parameters::get_headers(headers, full);
}

template <typename B>
module::Encoder_BCH<B>* Encoder_BCH::parameters
::build(const tools::BCH_polynomial_generator<B> &GF) const
{
	if (this->type == "BCH") return new module::Encoder_BCH<B>(this->K, this->N_cw, GF, this->n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B>
module::Encoder_BCH<B>* Encoder_BCH
::build(const parameters &params, const tools::BCH_polynomial_generator<B> &GF)
{
	return params.template build<B>(GF);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Encoder_BCH<B_8 >* aff3ct::factory::Encoder_BCH::parameters::build<B_8 >(const aff3ct::tools::BCH_polynomial_generator<B_8 >&) const;
template aff3ct::module::Encoder_BCH<B_16>* aff3ct::factory::Encoder_BCH::parameters::build<B_16>(const aff3ct::tools::BCH_polynomial_generator<B_16>&) const;
template aff3ct::module::Encoder_BCH<B_32>* aff3ct::factory::Encoder_BCH::parameters::build<B_32>(const aff3ct::tools::BCH_polynomial_generator<B_32>&) const;
template aff3ct::module::Encoder_BCH<B_64>* aff3ct::factory::Encoder_BCH::parameters::build<B_64>(const aff3ct::tools::BCH_polynomial_generator<B_64>&) const;
template aff3ct::module::Encoder_BCH<B_8 >* aff3ct::factory::Encoder_BCH::build<B_8 >(const aff3ct::factory::Encoder_BCH::parameters&, const aff3ct::tools::BCH_polynomial_generator<B_8 >&);
template aff3ct::module::Encoder_BCH<B_16>* aff3ct::factory::Encoder_BCH::build<B_16>(const aff3ct::factory::Encoder_BCH::parameters&, const aff3ct::tools::BCH_polynomial_generator<B_16>&);
template aff3ct::module::Encoder_BCH<B_32>* aff3ct::factory::Encoder_BCH::build<B_32>(const aff3ct::factory::Encoder_BCH::parameters&, const aff3ct::tools::BCH_polynomial_generator<B_32>&);
template aff3ct::module::Encoder_BCH<B_64>* aff3ct::factory::Encoder_BCH::build<B_64>(const aff3ct::factory::Encoder_BCH::parameters&, const aff3ct::tools::BCH_polynomial_generator<B_64>&);
#else
template aff3ct::module::Encoder_BCH<B>* aff3ct::factory::Encoder_BCH::parameters::build<B>(const aff3ct::tools::BCH_polynomial_generator<B>&) const;
template aff3ct::module::Encoder_BCH<B>* aff3ct::factory::Encoder_BCH::build<B>(const aff3ct::factory::Encoder_BCH::parameters&, const aff3ct::tools::BCH_polynomial_generator<B>&);
#endif
// ==================================================================================== explicit template instantiation
