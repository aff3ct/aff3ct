#include "Tools/Exception/exception.hpp"

#include "Module/Puncturer/Polar/Puncturer_polar_wangliu.hpp"

#include "Puncturer_polar.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Puncturer_polar::name   = "Puncturer Polar";
const std::string aff3ct::factory::Puncturer_polar::prefix = "pct";

template <typename B, typename Q>
module::Puncturer<B,Q>* Puncturer_polar
::build(const parameters                     &params,
        const tools::Frozenbits_generator<B> &fb_generator)
{
	if (params.type == "WANGLIU") return new module::Puncturer_polar_wangliu<B,Q>(params.K, params.N, fb_generator, params.n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

void Puncturer_polar
::build_args(arg_map &req_args, arg_map &opt_args, const std::string p)
{
	Puncturer::build_args(req_args, opt_args);

	opt_args[{p+"-type"}][2] += ", WANGLIU";
}

void Puncturer_polar
::store_args(const tools::Arguments_reader& ar, parameters &params, const std::string p)
{
	params.type = "WANGLIU";

	Puncturer::store_args(ar, params);
	params.N_cw = (int)std::exp2((int)std::ceil(std::log2(params.N)));
	params.R = (float)params.K / (float)params.N;

	if (params.N == params.N_cw)
		params.type = "NO";
}

void Puncturer_polar
::header(params_list& head_pct, const Puncturer::parameters& params)
{
	Puncturer::header(head_pct, params);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Puncturer<B_8 ,Q_8 >* aff3ct::factory::Puncturer_polar::build<B_8 ,Q_8 >(const aff3ct::factory::Puncturer::parameters&, const aff3ct::tools::Frozenbits_generator<B_8 >&);
template aff3ct::module::Puncturer<B_16,Q_16>* aff3ct::factory::Puncturer_polar::build<B_16,Q_16>(const aff3ct::factory::Puncturer::parameters&, const aff3ct::tools::Frozenbits_generator<B_16>&);
template aff3ct::module::Puncturer<B_32,Q_32>* aff3ct::factory::Puncturer_polar::build<B_32,Q_32>(const aff3ct::factory::Puncturer::parameters&, const aff3ct::tools::Frozenbits_generator<B_32>&);
template aff3ct::module::Puncturer<B_64,Q_64>* aff3ct::factory::Puncturer_polar::build<B_64,Q_64>(const aff3ct::factory::Puncturer::parameters&, const aff3ct::tools::Frozenbits_generator<B_64>&);
#else
template aff3ct::module::Puncturer<B,Q>* aff3ct::factory::Puncturer_polar::build<B,Q>(const aff3ct::factory::Puncturer::parameters&, const aff3ct::tools::Frozenbits_generator<B>&);
#endif
// ==================================================================================== explicit template instantiation
