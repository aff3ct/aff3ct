#include <sstream>

#include "Tools/Exception/exception.hpp"

#include "Module/Decoder/NO/Decoder_NO.hpp"

#include "Factory_decoder_NO.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B, typename Q>
Decoder<B,Q>* Factory_decoder_NO
::build(Factory_decoder::parameters &params)
{
	if (params.type == "NONE" && params.implem == "HARD_DECISION") return new Decoder_NO<B,Q>(params.K, params.n_frames);

	throw cannot_allocate(__FILE__, __LINE__, __func__);
}

void Factory_decoder_NO
::build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args)
{
	Factory_decoder::build_args(req_args, opt_args);

	opt_args[{"dec-type", "D"}].push_back("NONE");
	opt_args[{"dec-implem"}].push_back("HARD_DECISION");
}

void Factory_decoder_NO
::store_args(const Arguments_reader& ar, Factory_decoder::parameters &params)
{
	params.type   = "NONE";
	params.implem = "HARD_DECISION";

	Factory_decoder::store_args(ar, params);
}

void Factory_decoder_NO
::group_args(Arguments_reader::arg_grp& ar)
{
	Factory_decoder::group_args(ar);
}

void Factory_decoder_NO
::header(params_list& head_dec, const Factory_decoder::parameters& params)
{
	Factory_decoder::header(head_dec, params);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Decoder<B_8 ,Q_8 >* aff3ct::tools::Factory_decoder_NO::build<B_8 ,Q_8 >(aff3ct::tools::Factory_decoder::parameters&);
template aff3ct::module::Decoder<B_16,Q_16>* aff3ct::tools::Factory_decoder_NO::build<B_16,Q_16>(aff3ct::tools::Factory_decoder::parameters&);
template aff3ct::module::Decoder<B_32,Q_32>* aff3ct::tools::Factory_decoder_NO::build<B_32,Q_32>(aff3ct::tools::Factory_decoder::parameters&);
template aff3ct::module::Decoder<B_64,Q_64>* aff3ct::tools::Factory_decoder_NO::build<B_64,Q_64>(aff3ct::tools::Factory_decoder::parameters&);
#else
template aff3ct::module::Decoder<B,Q>* aff3ct::tools::Factory_decoder_NO::build<B,Q>(aff3ct::tools::Factory_decoder::parameters&);
#endif
// ==================================================================================== explicit template instantiation
