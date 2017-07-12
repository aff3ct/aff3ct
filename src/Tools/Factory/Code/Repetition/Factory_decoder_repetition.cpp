#include "Tools/Exception/exception.hpp"

#include "Module/Decoder/Repetition/Decoder_repetition_std.hpp"
#include "Module/Decoder/Repetition/Decoder_repetition_fast.hpp"

#include "Factory_decoder_repetition.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B, typename R>
Decoder<B,R>* Factory_decoder_repetition
::build(const Factory_decoder::parameters &params, const bool buffered)
{
	if (params.type == "REPETITION")
	{
		     if (params.implem == "STD" ) return new Decoder_repetition_std <B,R>(params.K, params.N, buffered, params.n_frames);
		else if (params.implem == "FAST") return new Decoder_repetition_fast<B,R>(params.K, params.N, buffered, params.n_frames);
	}

	throw cannot_allocate(__FILE__, __LINE__, __func__);
}

void Factory_decoder_repetition
::build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args)
{
	Factory_decoder::build_args(req_args, opt_args);

	// ------------------------------------------------------------------------------------------------------- decoder
	opt_args[{"dec-type", "D"}].push_back("REPETITION");
	opt_args[{"dec-implem"   }].push_back("STD, FAST");
}

void Factory_decoder_repetition
::store_args(const Arguments_reader& ar, Factory_decoder::parameters &params,
             const int K, const int N, const int n_frames)
{
	params.type   = "REPETITION";
	params.implem = "STD";

	Factory_decoder::store_args(ar, params, K, N, n_frames);
}

void Factory_decoder_repetition
::group_args(Arguments_reader::arg_grp& ar)
{
	Factory_decoder::group_args(ar);
}

void Factory_decoder_repetition
::header(Header::params_list& head_dec, const Factory_decoder::parameters& params)
{
	Factory_decoder::header(head_dec, params);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Decoder<B_8 ,Q_8 >* aff3ct::tools::Factory_decoder_repetition::build<B_8 ,Q_8 >(const aff3ct::tools::Factory_decoder::parameters&, const bool);
template aff3ct::module::Decoder<B_16,Q_16>* aff3ct::tools::Factory_decoder_repetition::build<B_16,Q_16>(const aff3ct::tools::Factory_decoder::parameters&, const bool);
template aff3ct::module::Decoder<B_32,Q_32>* aff3ct::tools::Factory_decoder_repetition::build<B_32,Q_32>(const aff3ct::tools::Factory_decoder::parameters&, const bool);
template aff3ct::module::Decoder<B_64,Q_64>* aff3ct::tools::Factory_decoder_repetition::build<B_64,Q_64>(const aff3ct::tools::Factory_decoder::parameters&, const bool);
#else
template aff3ct::module::Decoder<B,Q>* aff3ct::tools::Factory_decoder_repetition::build<B,Q>(const aff3ct::tools::Factory_decoder::parameters&, const bool);
#endif
// ==================================================================================== explicit template instantiation
