#include "Tools/Exception/exception.hpp"

#include "Module/Puncturer/NO/Puncturer_NO.hpp"
#include "Module/Puncturer/Turbo/Puncturer_turbo.hpp"

#include "Factory_puncturer_turbo.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B, typename Q>
Puncturer<B,Q>* Factory_puncturer_turbo<B,Q>
::build(const puncturer_parameters_turbo &params,
        const int                         tail_length,
        const bool                        buffered)
{
	if (params.type == "TURBO") return new Puncturer_turbo<B,Q>(params.K, params.N, tail_length, params.pattern, buffered, params.n_frames);

	throw cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q>
void Factory_puncturer_turbo<B,Q>
::build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args)
{
	Factory_puncturer::build_args(req_args, opt_args);

	// ----------------------------------------------------------------------------------------------------- poncturer
	opt_args[{"pct-pattern"}] =
		{"string",
		 "puncturing pattern for the turbo encoder (ex: \"11,10,01\")."};
}

template <typename B, typename Q>
void Factory_puncturer_turbo<B,Q>
::store_args(const Arguments_reader& ar, puncturer_parameters_turbo &params,
             const int K, const int N, const int N_pct, const int n_frames)
{
	params.type = "TURBO";

	Factory_puncturer::store_args(ar, params, K, N, N_pct, n_frames);

	// ----------------------------------------------------------------------------------------------------- poncturer
	if(ar.exist_arg({"pct-pattern"})) params.pattern = ar.get_arg({"pct-pattern"});
}

template <typename B, typename Q>
void Factory_puncturer_turbo<B,Q>
::group_args(Arguments_reader::arg_grp& ar)
{
	Factory_puncturer::group_args(ar);
}

template <typename B, typename Q>
void Factory_puncturer_turbo<B,Q>
::header(Header::params_list& head_pct, const puncturer_parameters_turbo& params)
{
	Factory_puncturer::header(head_pct, params);

	// ----------------------------------------------------------------------------------------------------- poncturer
	head_pct.push_back(std::make_pair(std::string("Pattern"), std::string("{" + params.pattern) + "}"));
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct aff3ct::tools::Factory_puncturer_turbo<B_8,Q_8>;
template struct aff3ct::tools::Factory_puncturer_turbo<B_16,Q_16>;
template struct aff3ct::tools::Factory_puncturer_turbo<B_32,Q_32>;
template struct aff3ct::tools::Factory_puncturer_turbo<B_64,Q_64>;
#else
template struct aff3ct::tools::Factory_puncturer_turbo<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
