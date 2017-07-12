#include "Tools/Exception/exception.hpp"

#include "Module/Encoder/BCH/Encoder_BCH.hpp"

#include "Factory_encoder_BCH.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B>
Encoder<B>* Factory_encoder_BCH
::build(const typename Factory_encoder_common::parameters &params, const Galois &GF)
{
	if (params.type == "BCH") return new Encoder_BCH<B>(params.K, params.N, GF, params.n_frames);

	throw cannot_allocate(__FILE__, __LINE__, __func__);
}

void Factory_encoder_BCH
::build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args)
{
	Factory_encoder_common::build_args(req_args, opt_args);

	// ------------------------------------------------------------------------------------------------------- encoder
	opt_args[{"enc-type"}][2] += ", BCH";
}

void Factory_encoder_BCH
::store_args(const Arguments_reader& ar, typename Factory_encoder_common::parameters &params,
             const int K, const int N, const int n_frames)
{
	params.type = "BCH";

	Factory_encoder_common::store_args(ar, params, K, N, n_frames);
}

void Factory_encoder_BCH
::group_args(Arguments_reader::arg_grp& ar)
{
	Factory_encoder_common::group_args(ar);
}

void Factory_encoder_BCH
::header(Header::params_list& head_enc, Header::params_list& head_cde, const typename Factory_encoder_common::parameters& params)
{
	Factory_encoder_common::header(head_enc, head_cde, params);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Encoder<B_8 >* aff3ct::tools::Factory_encoder_BCH::build<B_8 >(const aff3ct::tools::Factory_encoder_common::parameters&, const aff3ct::tools::Galois&);
template aff3ct::module::Encoder<B_16>* aff3ct::tools::Factory_encoder_BCH::build<B_16>(const aff3ct::tools::Factory_encoder_common::parameters&, const aff3ct::tools::Galois&);
template aff3ct::module::Encoder<B_32>* aff3ct::tools::Factory_encoder_BCH::build<B_32>(const aff3ct::tools::Factory_encoder_common::parameters&, const aff3ct::tools::Galois&);
template aff3ct::module::Encoder<B_64>* aff3ct::tools::Factory_encoder_BCH::build<B_64>(const aff3ct::tools::Factory_encoder_common::parameters&, const aff3ct::tools::Galois&);
#else
template aff3ct::module::Encoder<B>* aff3ct::tools::Factory_encoder_BCH::build<B>(const aff3ct::tools::Factory_encoder_common::parameters&, const aff3ct::tools::Galois&);
#endif
// ==================================================================================== explicit template instantiation
