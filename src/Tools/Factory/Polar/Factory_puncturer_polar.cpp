#include "Tools/Exception/exception.hpp"

#include "Module/Puncturer/NO/Puncturer_NO.hpp"
#include "Module/Puncturer/Polar/Puncturer_polar_wangliu.hpp"

#include "Factory_puncturer_polar.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B, typename Q>
Puncturer<B,Q>* Factory_puncturer_polar<B,Q>
::build(const typename Factory_puncturer::puncturer_parameters &params,
        const Frozenbits_generator<B>                          &fb_generator)
{
	if (params.type == "WANGLIU") return new Puncturer_polar_wangliu<B,Q>(params.K, params.N, fb_generator, params.n_frames);

	throw cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q>
void Factory_puncturer_polar<B,Q>
::build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args)
{
	Factory_puncturer::build_args(req_args, opt_args);
}

template <typename B, typename Q>
void Factory_puncturer_polar<B,Q>
::store_args(const Arguments_reader& ar, typename Factory_puncturer::puncturer_parameters &params,
             const int K, const int N, const int N_pct, const int n_frames)
{
	params.type = "WANGLIU";

	Factory_puncturer::store_args(ar, params, K, N, N_pct, n_frames);
}

template <typename B, typename Q>
void Factory_puncturer_polar<B,Q>
::group_args(Arguments_reader::arg_grp& ar)
{
	Factory_puncturer::group_args(ar);
}

template <typename B, typename Q>
void Factory_puncturer_polar<B,Q>
::header(Header::params_list& head_pct, const typename Factory_puncturer::puncturer_parameters& params)
{
	Factory_puncturer::header(head_pct, params);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct aff3ct::tools::Factory_puncturer_polar<B_8,Q_8>;
template struct aff3ct::tools::Factory_puncturer_polar<B_16,Q_16>;
template struct aff3ct::tools::Factory_puncturer_polar<B_32,Q_32>;
template struct aff3ct::tools::Factory_puncturer_polar<B_64,Q_64>;
#else
template struct aff3ct::tools::Factory_puncturer_polar<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
