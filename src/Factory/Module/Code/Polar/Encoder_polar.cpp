#include "Tools/Exception/exception.hpp"

#include "Module/Encoder/Polar/Encoder_polar.hpp"
#include "Module/Encoder/Polar/Encoder_polar_sys.hpp"

#include "Encoder_polar.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Encoder_polar::name   = "Encoder Polar";
const std::string aff3ct::factory::Encoder_polar::prefix = "enc";

template <typename B>
module::Encoder<B>* Encoder_polar
::build(const parameters& params, const mipp::vector<B> &frozen_bits)
{
	     if (params.type == "POLAR" && !params.systematic) return new module::Encoder_polar    <B>(params.K, params.N_cw, frozen_bits, params.n_frames);
	else if (params.type == "POLAR" &&  params.systematic) return new module::Encoder_polar_sys<B>(params.K, params.N_cw, frozen_bits, params.n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
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
::make_header(params_list& head_enc, const Encoder::parameters& params, const bool full)
{
	Encoder::make_header(head_enc, params, full);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Encoder<B_8 >* aff3ct::factory::Encoder_polar::build<B_8 >(const aff3ct::factory::Encoder::parameters&, const mipp::vector<B_8 >&);
template aff3ct::module::Encoder<B_16>* aff3ct::factory::Encoder_polar::build<B_16>(const aff3ct::factory::Encoder::parameters&, const mipp::vector<B_16>&);
template aff3ct::module::Encoder<B_32>* aff3ct::factory::Encoder_polar::build<B_32>(const aff3ct::factory::Encoder::parameters&, const mipp::vector<B_32>&);
template aff3ct::module::Encoder<B_64>* aff3ct::factory::Encoder_polar::build<B_64>(const aff3ct::factory::Encoder::parameters&, const mipp::vector<B_64>&);
#else
template aff3ct::module::Encoder<B>* aff3ct::factory::Encoder_polar::build<B>(const aff3ct::factory::Encoder::parameters&, const mipp::vector<B>&);
#endif
// ==================================================================================== explicit template instantiation
