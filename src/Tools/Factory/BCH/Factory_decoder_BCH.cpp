#include "Tools/Exception/exception.hpp"

#include "Module/Decoder/BCH/Decoder_BCH.hpp"

#include "Factory_decoder_BCH.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B, typename Q>
Decoder<B,Q>* Factory_decoder_BCH<B,Q>
::build(const std::string  type,
        const std::string  implem,
        const int          K,
        const int          N,
        const int          t,
        const Galois      &GF,
        const int          n_frames)
{
	if (type == "ALGEBRAIC")
	{
		if (implem == "STD") return new Decoder_BCH<B,Q>(K, N, t, GF, n_frames);
	}

	throw cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q>
void Factory_decoder_BCH<B,Q>
::build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args)
{
	Factory_decoder_common::build_args(req_args, opt_args);
}

template <typename B, typename Q>
void Factory_decoder_BCH<B,Q>
::store_args(const Arguments_reader& ar, decoder_parameters &params)
{
	params.type   = "ALGEBRAIC";
	params.implem = "STD";

	Factory_decoder_common::store_args(ar, params);
}

template <typename B, typename Q>
void Factory_decoder_BCH<B,Q>
::group_args(Arguments_reader::arg_grp& ar)
{
	Factory_decoder_common::group_args(ar);
}


// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct aff3ct::tools::Factory_decoder_BCH<B_8,Q_8>;
template struct aff3ct::tools::Factory_decoder_BCH<B_16,Q_16>;
template struct aff3ct::tools::Factory_decoder_BCH<B_32,Q_32>;
template struct aff3ct::tools::Factory_decoder_BCH<B_64,Q_64>;
#else
template struct aff3ct::tools::Factory_decoder_BCH<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
