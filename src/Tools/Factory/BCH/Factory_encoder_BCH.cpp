#include "Tools/Exception/exception.hpp"

#include "Module/Encoder/BCH/Encoder_BCH.hpp"

#include "Factory_encoder_BCH.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B>
Encoder<B>* Factory_encoder_BCH<B>
::build(const typename Factory_encoder_common<B>::encoder_parameters &params, const Galois &GF)
{
	if (params.type == "BCH") return new Encoder_BCH<B>(params.K, params.N, GF, params.n_frames);

	throw cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B>
void Factory_encoder_BCH<B>
::build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args)
{
	Factory_encoder_common<B>::build_args(req_args, opt_args);

	// ------------------------------------------------------------------------------------------------------- encoder
	opt_args[{"enc-type"}][2] += ", BCH";
}

template <typename B>
void Factory_encoder_BCH<B>
::store_args(const Arguments_reader& ar, typename Factory_encoder_common<B>::encoder_parameters &params,
             const int K, const int N, const int n_frames)
{
	params.type = "BCH";

	Factory_encoder_common<B>::store_args(ar, params, K, N, n_frames);
}

template <typename B>
void Factory_encoder_BCH<B>
::group_args(Arguments_reader::arg_grp& ar)
{
	Factory_encoder_common<B>::group_args(ar);
}

template <typename B>
void Factory_encoder_BCH<B>
::header(Header::params_list& head_enc, Header::params_list& head_cde, const typename Factory_encoder_common<B>::encoder_parameters& params)
{
	Factory_encoder_common<B>::header(head_enc, head_cde, params);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct aff3ct::tools::Factory_encoder_BCH<B_8>;
template struct aff3ct::tools::Factory_encoder_BCH<B_16>;
template struct aff3ct::tools::Factory_encoder_BCH<B_32>;
template struct aff3ct::tools::Factory_encoder_BCH<B_64>;
#else
template struct aff3ct::tools::Factory_encoder_BCH<B>;
#endif
// ==================================================================================== explicit template instantiation
