#include "Tools/Exception/exception.hpp"

#include "Module/Encoder/RA/Encoder_RA.hpp"

#include "Factory_encoder_RA.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B>
Encoder<B>* Factory_encoder_RA<B>
::build(const std::string       type,
        const int               K,
        const int               N,
        const Interleaver<int> &itl,
        const int               n_frames)
{
	if (type == "RA") return new Encoder_RA<B>(K, N, itl, n_frames);

	throw cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B>
void Factory_encoder_RA<B>
::build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args)
{
	Factory_encoder_common<B>::build_args(req_args, opt_args);

	// ------------------------------------------------------------------------------------------------------- encoder
	opt_args[{"enc-type"}][2] += ", RA";
}

template <typename B>
void Factory_encoder_RA<B>
::store_args(const Arguments_reader& ar, typename Factory_encoder_common<B>::encoder_parameters &params)
{
	params.type = "RA";

	Factory_encoder_common<B>::store_args(ar, params);
}

template <typename B>
void Factory_encoder_RA<B>
::group_args(Arguments_reader::arg_grp& ar)
{
	Factory_encoder_common<B>::group_args(ar);
}

template <typename B>
void Factory_encoder_RA<B>
::header(Header::params_list& head_enc, const typename Factory_encoder_common<B>::encoder_parameters& params)
{
	Factory_encoder_common<B>::header(head_enc, params);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct aff3ct::tools::Factory_encoder_RA<B_8>;
template struct aff3ct::tools::Factory_encoder_RA<B_16>;
template struct aff3ct::tools::Factory_encoder_RA<B_32>;
template struct aff3ct::tools::Factory_encoder_RA<B_64>;
#else
template struct aff3ct::tools::Factory_encoder_RA<B>;
#endif
// ==================================================================================== explicit template instantiation
