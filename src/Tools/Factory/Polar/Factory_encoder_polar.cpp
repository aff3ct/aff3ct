#include "Tools/Exception/exception.hpp"

#include "Module/Encoder/Polar/Encoder_polar.hpp"
#include "Module/Encoder/Polar/Encoder_polar_sys.hpp"

#include "Factory_encoder_polar.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B>
Encoder<B>* Factory_encoder_polar<B>
::build(const typename Factory_encoder_common<B>::encoder_parameters& params, const mipp::vector<B> &frozen_bits)
{
	     if (params.type == "POLAR" && !params.systematic) return new Encoder_polar    <B>(params.K, params.N, frozen_bits, params.n_frames);
	else if (params.type == "POLAR" &&  params.systematic) return new Encoder_polar_sys<B>(params.K, params.N, frozen_bits, params.n_frames);

	throw cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B>
void Factory_encoder_polar<B>
::build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args)
{
	Factory_encoder_common<B>::build_args(req_args, opt_args);

	// ------------------------------------------------------------------------------------------------------- encoder
	opt_args[{"enc-type"}][2] += ", POLAR";
}

template <typename B>
void Factory_encoder_polar<B>
::store_args(const Arguments_reader& ar, typename Factory_encoder_common<B>::encoder_parameters &params,
             const int K, const int N, const int n_frames)
{
	params.type = "POLAR";

	Factory_encoder_common<B>::store_args(ar, params, K, N, n_frames);
}

template <typename B>
void Factory_encoder_polar<B>
::group_args(Arguments_reader::arg_grp& ar)
{
	Factory_encoder_common<B>::group_args(ar);
}

template <typename B>
void Factory_encoder_polar<B>
::header(Header::params_list& head_enc, Header::params_list& head_cde, const typename Factory_encoder_common<B>::encoder_parameters& params)
{
	Factory_encoder_common<B>::header(head_enc, head_cde, params);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct aff3ct::tools::Factory_encoder_polar<B_8>;
template struct aff3ct::tools::Factory_encoder_polar<B_16>;
template struct aff3ct::tools::Factory_encoder_polar<B_32>;
template struct aff3ct::tools::Factory_encoder_polar<B_64>;
#else
template struct aff3ct::tools::Factory_encoder_polar<B>;
#endif
// ==================================================================================== explicit template instantiation
