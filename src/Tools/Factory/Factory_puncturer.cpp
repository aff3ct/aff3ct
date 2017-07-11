#include "Factory_puncturer.hpp"

#include "Module/Puncturer/NO/Puncturer_NO.hpp"

using namespace aff3ct;
using namespace tools;
using namespace module;

template <typename B, typename Q>
Puncturer<B,Q>* Factory_puncturer<B,Q>
::build(const puncturer_parameters &params)
{
	if (params.type == "NO") return new Puncturer_NO<B,Q>(params.K, params.N, params.n_frames);

	throw cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q>
void Factory_puncturer<B,Q>
::build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args)
{
	// ----------------------------------------------------------------------------------------------------- puncturer
}

template <typename B, typename Q>
void Factory_puncturer<B,Q>
::store_args(const Arguments_reader& ar, puncturer_parameters &params,
                                   const int K, const int N, const int N_pct, const int n_frames)
{
	params.K        = K;
	params.N        = N;
	params.N_pct    = N_pct;
	params.n_frames = n_frames;

	// ----------------------------------------------------------------------------------------------------- puncturer
	if (params.N == params.N_pct)
		params.type = "NO";
}

template <typename B, typename Q>
void Factory_puncturer<B,Q>
::group_args(Arguments_reader::arg_grp& ar)
{
	ar.push_back({"pct","Puncturer parameter(s)"});
}

template <typename B, typename Q>
void Factory_puncturer<B,Q>
::header(Header::params_list& head_pct, const puncturer_parameters& params)
{
	// ----------------------------------------------------------------------------------------------------- puncturer
}


// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct aff3ct::tools::Factory_puncturer<B_8,Q_8>;
template struct aff3ct::tools::Factory_puncturer<B_16,Q_16>;
template struct aff3ct::tools::Factory_puncturer<B_32,Q_32>;
template struct aff3ct::tools::Factory_puncturer<B_64,Q_64>;
#else
template struct aff3ct::tools::Factory_puncturer<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
