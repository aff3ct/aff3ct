#include "Tools/Exception/exception.hpp"

#include "Module/Encoder/BCH/Encoder_BCH.hpp"

#include "Encoder_BCH.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Encoder_BCH::name   = "Encoder BCH";
const std::string aff3ct::factory::Encoder_BCH::prefix = "enc";

template <typename B>
module::Encoder<B>* Encoder_BCH::parameters
::build(const tools::Galois &GF) const
{
	if (this->type == "BCH") return new module::Encoder_BCH<B>(this->K, this->N_cw, GF, this->n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B>
module::Encoder<B>* Encoder_BCH
::build(const parameters &params, const tools::Galois &GF)
{
	return params.template build<B>(GF);
}

void Encoder_BCH
::build_args(arg_map &req_args, arg_map &opt_args, const std::string p)
{
	Encoder::build_args(req_args, opt_args, p);

	opt_args[{p+"-type"}][2] += ", BCH";
}

void Encoder_BCH
::store_args(const arg_val_map &vals, parameters &params, const std::string p)
{
	params.type = "BCH";

	Encoder::store_args(vals, params, p);
}

void Encoder_BCH
::make_header(params_list& head_enc, const parameters& params, const bool full)
{
	Encoder::make_header(head_enc, params, full);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Encoder<B_8 >* aff3ct::factory::Encoder_BCH::parameters::build<B_8 >(const aff3ct::tools::Galois&) const;
template aff3ct::module::Encoder<B_16>* aff3ct::factory::Encoder_BCH::parameters::build<B_16>(const aff3ct::tools::Galois&) const;
template aff3ct::module::Encoder<B_32>* aff3ct::factory::Encoder_BCH::parameters::build<B_32>(const aff3ct::tools::Galois&) const;
template aff3ct::module::Encoder<B_64>* aff3ct::factory::Encoder_BCH::parameters::build<B_64>(const aff3ct::tools::Galois&) const;
template aff3ct::module::Encoder<B_8 >* aff3ct::factory::Encoder_BCH::build<B_8 >(const aff3ct::factory::Encoder_BCH::parameters&, const aff3ct::tools::Galois&);
template aff3ct::module::Encoder<B_16>* aff3ct::factory::Encoder_BCH::build<B_16>(const aff3ct::factory::Encoder_BCH::parameters&, const aff3ct::tools::Galois&);
template aff3ct::module::Encoder<B_32>* aff3ct::factory::Encoder_BCH::build<B_32>(const aff3ct::factory::Encoder_BCH::parameters&, const aff3ct::tools::Galois&);
template aff3ct::module::Encoder<B_64>* aff3ct::factory::Encoder_BCH::build<B_64>(const aff3ct::factory::Encoder_BCH::parameters&, const aff3ct::tools::Galois&);
#else
template aff3ct::module::Encoder<B>* aff3ct::factory::Encoder_BCH::parameters::build<B>(const aff3ct::tools::Galois&) const;
template aff3ct::module::Encoder<B>* aff3ct::factory::Encoder_BCH::build<B>(const aff3ct::factory::Encoder_BCH::parameters&, const aff3ct::tools::Galois&);
#endif
// ==================================================================================== explicit template instantiation
