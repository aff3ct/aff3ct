#include <sstream>
#include "Tools/Exception/exception.hpp"
#include "Module/Decoder/BCH/Decoder_BCH.hpp"
#include "Factory_decoder_BCH.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B, typename Q>
Decoder<B,Q>* Factory_decoder_BCH
::build(const parameters &params, const Galois &GF)
{
	if (params.type == "ALGEBRAIC")
	{
		if (params.implem == "STD") return new Decoder_BCH<B,Q>(params.K, params.N, params.t, GF, params.n_frames);
	}

	throw cannot_allocate(__FILE__, __LINE__, __func__);
}

void Factory_decoder_BCH
::build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args)
{
	Factory_decoder::build_args(req_args, opt_args);

	// ---------------------------------------------------------------------------------------------------------- code
	opt_args[{"cde-corr-pow", "T"}] =
		{"positive_int",
		 "correction power of the BCH code."};
}

void Factory_decoder_BCH
::store_args(const Arguments_reader& ar, parameters &params,
             const int K, const int N, const int n_frames)
{
	params.type     = "ALGEBRAIC";
	params.implem   = "STD";

	Factory_decoder::store_args(ar, params, K, N, n_frames);

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

void Factory_decoder_BCH
::group_args(Arguments_reader::arg_grp& ar)
{
	Factory_decoder::group_args(ar);
}

void Factory_decoder_BCH
::header(params_list& head_dec, params_list& head_cde, const parameters& params)
{
	Factory_decoder::header(head_dec, params);

	// ---------------------------------------------------------------------------------------------------------- code
	head_cde.push_back(std::make_pair("Galois field order (m)", std::to_string(params.m)));
	head_cde.push_back(std::make_pair("Correction power (T)",   std::to_string(params.t)));
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Decoder<B_8 ,Q_8 >* aff3ct::tools::Factory_decoder_BCH::build<B_8 ,Q_8 >(const aff3ct::tools::Factory_decoder_BCH::parameters&, const aff3ct::tools::Galois&);
template aff3ct::module::Decoder<B_16,Q_16>* aff3ct::tools::Factory_decoder_BCH::build<B_16,Q_16>(const aff3ct::tools::Factory_decoder_BCH::parameters&, const aff3ct::tools::Galois&);
template aff3ct::module::Decoder<B_32,Q_32>* aff3ct::tools::Factory_decoder_BCH::build<B_32,Q_32>(const aff3ct::tools::Factory_decoder_BCH::parameters&, const aff3ct::tools::Galois&);
template aff3ct::module::Decoder<B_64,Q_64>* aff3ct::tools::Factory_decoder_BCH::build<B_64,Q_64>(const aff3ct::tools::Factory_decoder_BCH::parameters&, const aff3ct::tools::Galois&);
#else
template aff3ct::module::Decoder<B,Q>* aff3ct::tools::Factory_decoder_BCH::build<B,Q>(const aff3ct::tools::Factory_decoder_BCH::parameters&, const aff3ct::tools::Galois&);
#endif
// ==================================================================================== explicit template instantiation

