#include <cmath>

#include "Tools/Exception/exception.hpp"

#include "Puncturer_polar.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Puncturer_polar::name   = "Puncturer Polar";
const std::string aff3ct::factory::Puncturer_polar::prefix = "pct";

template <typename B, typename Q>
module::Puncturer_polar_wangliu<B,Q>* Puncturer_polar::parameters
::build(const tools::Frozenbits_generator &fb_generator) const
{
	if (this->type == "WANGLIU") return new module::Puncturer_polar_wangliu<B,Q>(this->K, this->N, fb_generator, this->n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q>
module::Puncturer_polar_wangliu<B,Q>* Puncturer_polar
::build(const parameters                  &params,
        const tools::Frozenbits_generator &fb_generator)
{
	return params.template build<B,Q>(fb_generator);
}

void Puncturer_polar
::build_args(arg_map &req_args, arg_map &opt_args, const std::string p)
{
	Puncturer::build_args(req_args, opt_args, p);

	opt_args[{p+"-type"}][2] += ", WANGLIU";
}

void Puncturer_polar
::store_args(const arg_val_map &vals, parameters &params, const std::string p)
{
	params.type = "WANGLIU";

	Puncturer::store_args(vals, params, p);
	params.N_cw = (int)std::exp2((int)std::ceil(std::log2(params.N)));

	if (params.N == params.N_cw)
		params.type = "NO";
}

void Puncturer_polar
::make_header(params_list& head_pct, const parameters& params, const bool full)
{
	Puncturer::make_header(head_pct, params, full);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Puncturer_polar_wangliu<B_8 ,Q_8 >* aff3ct::factory::Puncturer_polar::parameters::build<B_8 ,Q_8 >(const aff3ct::tools::Frozenbits_generator&) const;
template aff3ct::module::Puncturer_polar_wangliu<B_16,Q_16>* aff3ct::factory::Puncturer_polar::parameters::build<B_16,Q_16>(const aff3ct::tools::Frozenbits_generator&) const;
template aff3ct::module::Puncturer_polar_wangliu<B_32,Q_32>* aff3ct::factory::Puncturer_polar::parameters::build<B_32,Q_32>(const aff3ct::tools::Frozenbits_generator&) const;
template aff3ct::module::Puncturer_polar_wangliu<B_64,Q_64>* aff3ct::factory::Puncturer_polar::parameters::build<B_64,Q_64>(const aff3ct::tools::Frozenbits_generator&) const;
template aff3ct::module::Puncturer_polar_wangliu<B_8 ,Q_8 >* aff3ct::factory::Puncturer_polar::build<B_8 ,Q_8 >(const aff3ct::factory::Puncturer_polar::parameters&, const aff3ct::tools::Frozenbits_generator&);
template aff3ct::module::Puncturer_polar_wangliu<B_16,Q_16>* aff3ct::factory::Puncturer_polar::build<B_16,Q_16>(const aff3ct::factory::Puncturer_polar::parameters&, const aff3ct::tools::Frozenbits_generator&);
template aff3ct::module::Puncturer_polar_wangliu<B_32,Q_32>* aff3ct::factory::Puncturer_polar::build<B_32,Q_32>(const aff3ct::factory::Puncturer_polar::parameters&, const aff3ct::tools::Frozenbits_generator&);
template aff3ct::module::Puncturer_polar_wangliu<B_64,Q_64>* aff3ct::factory::Puncturer_polar::build<B_64,Q_64>(const aff3ct::factory::Puncturer_polar::parameters&, const aff3ct::tools::Frozenbits_generator&);
#else
template aff3ct::module::Puncturer_polar_wangliu<B,Q>* aff3ct::factory::Puncturer_polar::parameters::build<B,Q>(const aff3ct::tools::Frozenbits_generator&) const;
template aff3ct::module::Puncturer_polar_wangliu<B,Q>* aff3ct::factory::Puncturer_polar::build<B,Q>(const aff3ct::factory::Puncturer_polar::parameters&, const aff3ct::tools::Frozenbits_generator&);
#endif
// ==================================================================================== explicit template instantiation
