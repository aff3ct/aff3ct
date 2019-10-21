#include "Tools/Exception/exception.hpp"
#include "Tools/Math/utils.h"
#include "Module/Encoder/BCH/Encoder_BCH.hpp"
#include "Factory/Module/Encoder/BCH/Encoder_BCH.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Encoder_BCH_name   = "Encoder BCH";
const std::string aff3ct::factory::Encoder_BCH_prefix = "enc";

Encoder_BCH
::Encoder_BCH(const std::string &prefix)
: Encoder(Encoder_BCH_name, prefix)
{
	this->type = "BCH";
}

Encoder_BCH* Encoder_BCH
::clone() const
{
	return new Encoder_BCH(*this);
}

void Encoder_BCH
::get_description(cli::Argument_map_info &args) const
{
	Encoder::get_description(args);

	auto p = this->get_prefix();

	cli::add_options(args.at({p+"-type"}), 0, "BCH");
}

void Encoder_BCH
::store(const cli::Argument_map_value &vals)
{
	Encoder::store(vals);
}

void Encoder_BCH
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	Encoder::get_headers(headers, full);
}

template <typename B>
module::Encoder_BCH<B>* Encoder_BCH
::build(const tools::BCH_polynomial_generator<B> &GF) const
{
	if (this->type == "BCH") return new module::Encoder_BCH<B>(this->K, this->N_cw, GF, this->n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::module::Encoder_BCH<B_8 >* aff3ct::factory::Encoder_BCH::build<B_8 >(const aff3ct::tools::BCH_polynomial_generator<B_8 >&) const;
template aff3ct::module::Encoder_BCH<B_16>* aff3ct::factory::Encoder_BCH::build<B_16>(const aff3ct::tools::BCH_polynomial_generator<B_16>&) const;
template aff3ct::module::Encoder_BCH<B_32>* aff3ct::factory::Encoder_BCH::build<B_32>(const aff3ct::tools::BCH_polynomial_generator<B_32>&) const;
template aff3ct::module::Encoder_BCH<B_64>* aff3ct::factory::Encoder_BCH::build<B_64>(const aff3ct::tools::BCH_polynomial_generator<B_64>&) const;
#else
template aff3ct::module::Encoder_BCH<B>* aff3ct::factory::Encoder_BCH::build<B>(const aff3ct::tools::BCH_polynomial_generator<B>&) const;
#endif
// ==================================================================================== explicit template instantiation
