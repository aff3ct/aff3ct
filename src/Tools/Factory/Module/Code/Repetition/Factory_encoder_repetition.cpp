#include "Tools/Exception/exception.hpp"

#include "Module/Encoder/Repetition/Encoder_repetition_sys.hpp"

#include "Factory_encoder_repetition.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B>
Encoder_sys<B>* Factory_encoder_repetition
::build(const parameters &params)
{
	if (params.type == "REPETITION") return new Encoder_repetition_sys<B>(params.K, params.N_cw, params.buffered, params.n_frames);

	throw cannot_allocate(__FILE__, __LINE__, __func__);
}

void Factory_encoder_repetition
::build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args)
{
	Factory_encoder::build_args(req_args, opt_args);

	opt_args[{"enc-type"}][2] += ", REPETITION";

	opt_args[{"enc-no-buff"}] =
		{"",
		 "disable the buffered encoding."};
}

void Factory_encoder_repetition
::store_args(const Arguments_reader& ar, parameters &params)
{
	params.type = "REPETITION";

	Factory_encoder::store_args(ar, params);

	if(ar.exist_arg({"enc-no-buff"})) params.buffered = false;
}

void Factory_encoder_repetition
::group_args(Arguments_reader::arg_grp& ar)
{
	Factory_encoder::group_args(ar);
}

void Factory_encoder_repetition
::header(params_list& head_enc, const parameters& params)
{
	Factory_encoder::header(head_enc, params);

	head_enc.push_back(std::make_pair("Buffered", (params.buffered ? "on" : "off")));
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Encoder_sys<B_8 >* aff3ct::tools::Factory_encoder_repetition::build<B_8 >(const aff3ct::tools::Factory_encoder_repetition::parameters&);
template aff3ct::module::Encoder_sys<B_16>* aff3ct::tools::Factory_encoder_repetition::build<B_16>(const aff3ct::tools::Factory_encoder_repetition::parameters&);
template aff3ct::module::Encoder_sys<B_32>* aff3ct::tools::Factory_encoder_repetition::build<B_32>(const aff3ct::tools::Factory_encoder_repetition::parameters&);
template aff3ct::module::Encoder_sys<B_64>* aff3ct::tools::Factory_encoder_repetition::build<B_64>(const aff3ct::tools::Factory_encoder_repetition::parameters&);
#else
template aff3ct::module::Encoder_sys<B>* aff3ct::tools::Factory_encoder_repetition::build<B>(const aff3ct::tools::Factory_encoder_repetition::parameters&);
#endif
// ==================================================================================== explicit template instantiation

