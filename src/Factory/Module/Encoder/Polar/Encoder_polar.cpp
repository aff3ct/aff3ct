#include "Tools/Exception/exception.hpp"

#include "Module/Encoder/Polar/Encoder_polar.hpp"
#include "Module/Encoder/Polar/Encoder_polar_sys.hpp"

#include "Encoder_polar.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Encoder_polar::name   = "Encoder Polar";
const std::string aff3ct::factory::Encoder_polar::prefix = "enc";

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

void Encoder_polar
::build_args(arg_map &req_args, arg_map &opt_args, const std::string p)
{
	Encoder::build_args(req_args, opt_args, p);

	opt_args[{p+"-type"}][2] += ", POLAR";

	opt_args[{p+"-no-sys"}] =
		{"",
		 "disable the systematic encoding."};
}

void Encoder_polar
::store_args(const arg_val_map &vals, parameters &params, const std::string p)
{
	params.type = "POLAR";

	Encoder::store_args(vals, params, p);
}

void Encoder_polar
::make_header(params_list& head_enc, const parameters& params, const bool full)
{
	Encoder::make_header(head_enc, params, full);
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
