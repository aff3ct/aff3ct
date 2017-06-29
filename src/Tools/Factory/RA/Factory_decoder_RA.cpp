#include "Tools/Exception/exception.hpp"

#include "Module/Decoder/RA/Decoder_RA.hpp"

#include "Factory_decoder_RA.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B, typename R>
Decoder<B,R>* Factory_decoder_RA<B,R>
::build(const std::string       type,
        const std::string       implem,
        const int               K,
        const int               N,
        const Interleaver<int> &itl,
        const int               n_ite,
        const int               n_frames)
{
	if (type == "RA")
	{
		if (implem == "STD" ) return new Decoder_RA<B,R>(K, N, itl, n_ite, n_frames);
	}

	throw cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q>
void Factory_decoder_RA<B,Q>
::build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args)
{
	Factory_decoder_common::build_args(req_args, opt_args);

	// --------------------------------------------------------------------------------------------------- interleaver
	Factory_interleaver<int>::build_args(req_args, opt_args);

	// ------------------------------------------------------------------------------------------------------- decoder
	opt_args[{"dec-ite", "i"}] =
		{"positive_int",
		 "maximal number of iterations in the decoder."};

	opt_args[{"dec-type", "D"}].push_back("RA" );

	opt_args[{"dec-implem"   }].push_back("STD");
}

template <typename B, typename Q>
void Factory_decoder_RA<B,Q>
::store_args(const Arguments_reader& ar, decoder_parameters_RA &params, const int seed)
{
	params.type   = "RA";
	params.implem = "STD";

	Factory_decoder_common::store_args(ar, params);

	// --------------------------------------------------------------------------------------------------- interleaver
	Factory_interleaver<int>::store_args(ar, params.itl, seed);

	// ------------------------------------------------------------------------------------------------------- decoder
	if(ar.exist_arg({"dec-ite", "i"})) params.n_ite = ar.get_arg_int({"dec-ite", "i"});
}

template <typename B, typename Q>
void Factory_decoder_RA<B,Q>
::group_args(Arguments_reader::arg_grp& ar)
{
	Factory_decoder_common::group_args(ar);

	// --------------------------------------------------------------------------------------------------- interleaver
	Factory_interleaver<int>::group_args(ar);
}

template <typename B, typename Q>
void Factory_decoder_RA<B,Q>
::header(Header::params_list& head_dec, Header::params_list& head_itl, const decoder_parameters_RA& params)
{
	Factory_decoder_common::header(head_dec, params);

	// --------------------------------------------------------------------------------------------------- interleaver
	Factory_interleaver<int>::header(head_itl, params.itl);

	// ------------------------------------------------------------------------------------------------------- decoder
	head_dec.push_back(std::make_pair("Num. of iterations (i)", std::to_string(params.n_ite)));
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct aff3ct::tools::Factory_decoder_RA<B_8,Q_8>;
template struct aff3ct::tools::Factory_decoder_RA<B_16,Q_16>;
template struct aff3ct::tools::Factory_decoder_RA<B_32,Q_32>;
template struct aff3ct::tools::Factory_decoder_RA<B_64,Q_64>;
#else
template struct aff3ct::tools::Factory_decoder_RA<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
