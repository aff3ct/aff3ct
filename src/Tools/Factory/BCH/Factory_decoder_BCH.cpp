#include <sstream>
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

	// ---------------------------------------------------------------------------------------------------------- code
	opt_args[{"cde-corr-pow", "T"}] =
		{"positive_int",
		 "correction power of the BCH code."};
}

template <typename B, typename Q>
void Factory_decoder_BCH<B,Q>
::store_args(const Arguments_reader& ar, decoder_parameters_BCH &params, int K, int N)
{
	params.type   = "ALGEBRAIC";
	params.implem = "STD";

	Factory_decoder_common::store_args(ar, params);

	// ---------------------------------------------------------------------------------------------------------- code
	params.m = (int)std::ceil(std::log2(N));
	if (params.m == 0)
	{
		std::stringstream message;
		message << "The Gallois Field order is null (because N = " << N << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (ar.exist_arg({"cde-corr-pow", "T"}))
		params.t = ar.get_arg_int({"cde-corr-pow", "T"});
	else
		params.t = (N - K) / params.m;
}

template <typename B, typename Q>
void Factory_decoder_BCH<B,Q>
::group_args(Arguments_reader::arg_grp& ar)
{
	Factory_decoder_common::group_args(ar);
}

template <typename B, typename Q>
void Factory_decoder_BCH<B,Q>
::header(Header::params_list& head_dec, Header::params_list& head_cde, const decoder_parameters_BCH& params)
{
	Factory_decoder_common::header(head_dec, params);

	// ---------------------------------------------------------------------------------------------------------- code
	head_cde.push_back(std::make_pair("Galois field order (m)", std::to_string(params.m)));
	head_cde.push_back(std::make_pair("Correction power (T)",   std::to_string(params.t)));
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
