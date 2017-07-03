#include "Tools/Exception/exception.hpp"

#include "Module/Encoder/Repetition/Encoder_repetition_sys.hpp"

#include "Factory_encoder_repetition.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B>
Encoder_sys<B>* Factory_encoder_repetition<B>
::build(const encoder_parameters_repetition &params)
{
	if (params.type == "REPETITION") return new Encoder_repetition_sys<B>(params.K, params.N, params.buffered, params.n_frames);

	throw cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B>
void Factory_encoder_repetition<B>
::build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args)
{
	Factory_encoder_common<B>::build_args(req_args, opt_args);

	// ------------------------------------------------------------------------------------------------------- encoder
	opt_args[{"enc-type"}][2] += ", REPETITION";

	opt_args[{"enc-no-buff"}] =
		{"",
		 "disable the buffered encoding."};
}

template <typename B>
void Factory_encoder_repetition<B>
::store_args(const Arguments_reader& ar, encoder_parameters_repetition &params,
             const int K, const int N, const int n_frames)
{
	params.type = "REPETITION";

	Factory_encoder_common<B>::store_args(ar, params, K, N, n_frames);

	// ------------------------------------------------------------------------------------------------------- encoder
	if(ar.exist_arg({"enc-no-buff"})) params.buffered = false;
}

template <typename B>
void Factory_encoder_repetition<B>
::group_args(Arguments_reader::arg_grp& ar)
{
	Factory_encoder_common<B>::group_args(ar);
}

template <typename B>
void Factory_encoder_repetition<B>
::header(Header::params_list& head_enc, const encoder_parameters_repetition& params)
{
	Factory_encoder_common<B>::header(head_enc, params);

	// ------------------------------------------------------------------------------------------------------- encoder
	head_enc.push_back(std::make_pair("Buffered", (params.buffered ? "on" : "off")));
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct aff3ct::tools::Factory_encoder_repetition<B_8>;
template struct aff3ct::tools::Factory_encoder_repetition<B_16>;
template struct aff3ct::tools::Factory_encoder_repetition<B_32>;
template struct aff3ct::tools::Factory_encoder_repetition<B_64>;
#else
template struct aff3ct::tools::Factory_encoder_repetition<B>;
#endif
// ==================================================================================== explicit template instantiation
