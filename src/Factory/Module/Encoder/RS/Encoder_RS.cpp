#include "Tools/Exception/exception.hpp"
#include "Tools/Math/utils.h"
#include "Module/Encoder/RS/Encoder_RS.hpp"
#include "Factory/Module/Encoder/RS/Encoder_RS.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Encoder_RS_name   = "Encoder RS";
const std::string aff3ct::factory::Encoder_RS_prefix = "enc";

Encoder_RS
::Encoder_RS(const std::string &prefix)
: Encoder(Encoder_RS_name, prefix)
{
	this->type = "RS";
}

Encoder_RS* Encoder_RS
::clone() const
{
	return new Encoder_RS(*this);
}

void Encoder_RS
::get_description(cli::Argument_map_info &args) const
{
	Encoder::get_description(args);

	auto p = this->get_prefix();

	cli::add_options(args.at({p+"-type"}), 0, "RS");
}

void Encoder_RS
::store(const cli::Argument_map_value &vals)
{
	Encoder::store(vals);
}

void Encoder_RS
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	Encoder::get_headers(headers, full);
}

template <typename B>
module::Encoder_RS<B>* Encoder_RS
::build(const tools::RS_polynomial_generator &GF) const
{
	if (this->type == "RS") return new module::Encoder_RS<B>(this->K, this->N_cw, GF, this->n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::module::Encoder_RS<B_8 >* aff3ct::factory::Encoder_RS::build<B_8 >(const aff3ct::tools::RS_polynomial_generator&) const;
template aff3ct::module::Encoder_RS<B_16>* aff3ct::factory::Encoder_RS::build<B_16>(const aff3ct::tools::RS_polynomial_generator&) const;
template aff3ct::module::Encoder_RS<B_32>* aff3ct::factory::Encoder_RS::build<B_32>(const aff3ct::tools::RS_polynomial_generator&) const;
template aff3ct::module::Encoder_RS<B_64>* aff3ct::factory::Encoder_RS::build<B_64>(const aff3ct::tools::RS_polynomial_generator&) const;
#else
template aff3ct::module::Encoder_RS<B>* aff3ct::factory::Encoder_RS::build<B>(const aff3ct::tools::RS_polynomial_generator&) const;
#endif
// ==================================================================================== explicit template instantiation
