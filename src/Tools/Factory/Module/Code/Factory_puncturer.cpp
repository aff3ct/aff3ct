#include "Factory_puncturer.hpp"

#include "Module/Puncturer/NO/Puncturer_NO.hpp"

using namespace aff3ct;
using namespace tools;
using namespace module;

template <typename B, typename Q>
Puncturer<B,Q>* Factory_puncturer
::build(const parameters &params)
{
	if (params.type == "NO") return new Puncturer_NO<B,Q>(params.K, params.N, params.n_frames);

	throw cannot_allocate(__FILE__, __LINE__, __func__);
}

void Factory_puncturer
::build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args)
{
	// ----------------------------------------------------------------------------------------------------- puncturer
}

void Factory_puncturer
::store_args(const Arguments_reader& ar, parameters &params,
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

void Factory_puncturer
::group_args(Arguments_reader::arg_grp& ar)
{
	ar.push_back({"pct","Puncturer parameter(s)"});
}

void Factory_puncturer
::header(params_list& head_pct, const parameters& params)
{
	// ----------------------------------------------------------------------------------------------------- puncturer
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Puncturer<B_8 ,Q_8 >* aff3ct::tools::Factory_puncturer::build<B_8 ,Q_8 >(const aff3ct::tools::Factory_puncturer::parameters&);
template aff3ct::module::Puncturer<B_16,Q_16>* aff3ct::tools::Factory_puncturer::build<B_16,Q_16>(const aff3ct::tools::Factory_puncturer::parameters&);
template aff3ct::module::Puncturer<B_32,Q_32>* aff3ct::tools::Factory_puncturer::build<B_32,Q_32>(const aff3ct::tools::Factory_puncturer::parameters&);
template aff3ct::module::Puncturer<B_64,Q_64>* aff3ct::tools::Factory_puncturer::build<B_64,Q_64>(const aff3ct::tools::Factory_puncturer::parameters&);
#else
template aff3ct::module::Puncturer<B,Q>* aff3ct::tools::Factory_puncturer::build<B,Q>(const aff3ct::tools::Factory_puncturer::parameters&);
#endif
// ==================================================================================== explicit template instantiation
