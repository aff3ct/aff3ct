#include "Tools/Exception/exception.hpp"

#include "Module/Encoder/RA/Encoder_RA.hpp"

#include "Factory_encoder_RA.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B>
Encoder<B>* Factory_encoder_RA
::build(const Factory_encoder::parameters& params, const Interleaver<int> &itl)
{
	if (params.type == "RA") return new Encoder_RA<B>(params.K, params.N_cw, itl, params.n_frames);

	throw cannot_allocate(__FILE__, __LINE__, __func__);
}

void Factory_encoder_RA
::build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args)
{
	Factory_encoder::build_args(req_args, opt_args);

	opt_args[{"enc-type"}][2] += ", RA";
}

void Factory_encoder_RA
::store_args(const Arguments_reader& ar, Factory_encoder::parameters &params)
{
	params.type = "RA";

	Factory_encoder::store_args(ar, params);
}

void Factory_encoder_RA
::group_args(Arguments_reader::arg_grp& ar)
{
	Factory_encoder::group_args(ar);
}

void Factory_encoder_RA
::header(params_list& head_enc, const Factory_encoder::parameters& params)
{
	Factory_encoder::header(head_enc, params);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Encoder<B_8 >* aff3ct::tools::Factory_encoder_RA::build<B_8 >(const aff3ct::tools::Factory_encoder::parameters&, const Interleaver<int>&);
template aff3ct::module::Encoder<B_16>* aff3ct::tools::Factory_encoder_RA::build<B_16>(const aff3ct::tools::Factory_encoder::parameters&, const Interleaver<int>&);
template aff3ct::module::Encoder<B_32>* aff3ct::tools::Factory_encoder_RA::build<B_32>(const aff3ct::tools::Factory_encoder::parameters&, const Interleaver<int>&);
template aff3ct::module::Encoder<B_64>* aff3ct::tools::Factory_encoder_RA::build<B_64>(const aff3ct::tools::Factory_encoder::parameters&, const Interleaver<int>&);
#else
template aff3ct::module::Encoder<B>* aff3ct::tools::Factory_encoder_RA::build<B>(const aff3ct::tools::Factory_encoder::parameters&, const Interleaver<int>&);
#endif
// ==================================================================================== explicit template instantiation
