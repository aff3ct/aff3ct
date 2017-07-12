#include "Tools/Exception/exception.hpp"

#include "Module/Puncturer/Polar/Puncturer_polar_wangliu.hpp"

#include "Factory_puncturer_polar.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B, typename Q>
Puncturer<B,Q>* Factory_puncturer_polar
::build(const Factory_puncturer::parameters &params,
        const Frozenbits_generator<B>       &fb_generator)
{
	if (params.type == "WANGLIU") return new Puncturer_polar_wangliu<B,Q>(params.K, params.N, fb_generator, params.n_frames);

	throw cannot_allocate(__FILE__, __LINE__, __func__);
}

void Factory_puncturer_polar
::build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args)
{
	Factory_puncturer::build_args(req_args, opt_args);
}

void Factory_puncturer_polar
::store_args(const Arguments_reader& ar, Factory_puncturer::parameters &params,
             const int K, const int N, const int N_pct, const int n_frames)
{
	params.type = "WANGLIU";

	Factory_puncturer::store_args(ar, params, K, N, N_pct, n_frames);
}

void Factory_puncturer_polar
::group_args(Arguments_reader::arg_grp& ar)
{
	Factory_puncturer::group_args(ar);
}

void Factory_puncturer_polar
::header(Header::params_list& head_pct, const Factory_puncturer::parameters& params)
{
	Factory_puncturer::header(head_pct, params);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Puncturer<B_8 ,Q_8 >* aff3ct::tools::Factory_puncturer_polar::build<B_8 ,Q_8 >(const aff3ct::tools::Factory_puncturer::parameters&, const Frozenbits_generator<B_8 >&);
template aff3ct::module::Puncturer<B_16,Q_16>* aff3ct::tools::Factory_puncturer_polar::build<B_16,Q_16>(const aff3ct::tools::Factory_puncturer::parameters&, const Frozenbits_generator<B_16>&);
template aff3ct::module::Puncturer<B_32,Q_32>* aff3ct::tools::Factory_puncturer_polar::build<B_32,Q_32>(const aff3ct::tools::Factory_puncturer::parameters&, const Frozenbits_generator<B_32>&);
template aff3ct::module::Puncturer<B_64,Q_64>* aff3ct::tools::Factory_puncturer_polar::build<B_64,Q_64>(const aff3ct::tools::Factory_puncturer::parameters&, const Frozenbits_generator<B_64>&);
#else
template aff3ct::module::Puncturer<B,Q>* aff3ct::tools::Factory_puncturer_polar::build<B,Q>(const aff3ct::tools::Factory_puncturer::parameters&, const Frozenbits_generator<B>&);
#endif
// ==================================================================================== explicit template instantiation
