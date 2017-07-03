#include "Tools/Exception/exception.hpp"

#include "Module/Decoder/Repetition/Decoder_repetition_std.hpp"
#include "Module/Decoder/Repetition/Decoder_repetition_fast.hpp"

#include "Factory_decoder_repetition.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B, typename R>
Decoder<B,R>* Factory_decoder_repetition<B,R>
::build(const typename Factory_decoder_common::decoder_parameters &params, const bool buffered = true)
{
	if (params.type == "REPETITION")
	{
		     if (params.implem == "STD" ) return new Decoder_repetition_std <B,R>(params.K, params.N, buffered, params.n_frames);
		else if (params.implem == "FAST") return new Decoder_repetition_fast<B,R>(params.K, params.N, buffered, params.n_frames);
	}

	throw cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q>
void Factory_decoder_repetition<B,Q>
::build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args)
{
	Factory_decoder_common::build_args(req_args, opt_args);

	// ------------------------------------------------------------------------------------------------------- decoder
	opt_args[{"dec-type", "D"}].push_back("REPETITION");
	opt_args[{"dec-implem"   }].push_back("STD, FAST");
}

template <typename B, typename Q>
void Factory_decoder_repetition<B,Q>
::store_args(const Arguments_reader& ar, typename Factory_decoder_common::decoder_parameters &params,
             const int K, const int N, const int n_frames)
{
	params.type   = "REPETITION";
	params.implem = "STD";

	Factory_decoder_common::store_args(ar, params, K, N, n_frames);
}

template <typename B, typename Q>
void Factory_decoder_repetition<B,Q>
::group_args(Arguments_reader::arg_grp& ar)
{
	Factory_decoder_common::group_args(ar);
}

template <typename B, typename Q>
void Factory_decoder_repetition<B,Q>
::header(Header::params_list& head_dec, const typename Factory_decoder_common::decoder_parameters& params)
{
	Factory_decoder_common::header(head_dec, params);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct aff3ct::tools::Factory_decoder_repetition<B_8,Q_8>;
template struct aff3ct::tools::Factory_decoder_repetition<B_16,Q_16>;
template struct aff3ct::tools::Factory_decoder_repetition<B_32,Q_32>;
template struct aff3ct::tools::Factory_decoder_repetition<B_64,Q_64>;
#else
template struct aff3ct::tools::Factory_decoder_repetition<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
