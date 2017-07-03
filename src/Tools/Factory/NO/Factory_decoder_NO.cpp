#include <sstream>
#include "Tools/Exception/exception.hpp"
#include "Module/Decoder/NO/Decoder_NO.hpp"
#include "Factory_decoder_NO.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B, typename Q>
Decoder<B,Q>* Factory_decoder_NO<B,Q>
::build(const std::string  type,
        const std::string  implem,
        const int          K,
        const int          n_frames)
{
	if (type == "NONE" && implem == "HARD_DECISION") return new Decoder_NO<B,Q>(K, n_frames);

	throw cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q>
void Factory_decoder_NO<B,Q>
::build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args)
{
	Factory_decoder_common::build_args(req_args, opt_args);

	// ------------------------------------------------------------------------------------------------------- decoder
	opt_args[{"dec-type", "D"}].push_back("NONE");
	opt_args[{"dec-implem"   }].push_back("HARD_DECISION");
}

template <typename B, typename Q>
void Factory_decoder_NO<B,Q>
::store_args(const Arguments_reader& ar, decoder_parameters &params,
             const int K, const int N, const int n_frames)
{
	params.type   = "NONE";
	params.implem = "HARD_DECISION";

	Factory_decoder_common::store_args(ar, params, K, N, n_frames);
}

template <typename B, typename Q>
void Factory_decoder_NO<B,Q>
::group_args(Arguments_reader::arg_grp& ar)
{
	Factory_decoder_common::group_args(ar);
}

template <typename B, typename Q>
void Factory_decoder_NO<B,Q>
::header(Header::params_list& head_dec, const decoder_parameters& params)
{
	Factory_decoder_common::header(head_dec, params);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct aff3ct::tools::Factory_decoder_NO<B_8,Q_8>;
template struct aff3ct::tools::Factory_decoder_NO<B_16,Q_16>;
template struct aff3ct::tools::Factory_decoder_NO<B_32,Q_32>;
template struct aff3ct::tools::Factory_decoder_NO<B_64,Q_64>;
#else
template struct aff3ct::tools::Factory_decoder_NO<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
