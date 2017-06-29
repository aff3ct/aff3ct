#include "Tools/Exception/exception.hpp"

#include "Module/Encoder/RSC/Encoder_RSC_generic_sys.hpp"
#include "Module/Encoder/RSC/Encoder_RSC_generic_json_sys.hpp"

#include "Factory_encoder_RSC.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B>
Encoder_RSC_sys<B>* Factory_encoder_RSC<B>
::build(const std::string       type,
        const int               K,
        const int               N,
        const bool              buffered,
        const std::vector<int>  poly,
              std::ostream     &stream,
        const int               n_frames)
{
	     if (type == "RSC_JSON") return new Encoder_RSC_generic_json_sys<B>(K, N, buffered, poly, stream, n_frames);
	else if (type == "RSC"     ) return new Encoder_RSC_generic_sys     <B>(K, N, buffered, poly,         n_frames);

	throw cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B>
void Factory_encoder_RSC<B>
::build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args)
{
	Factory_encoder_common<B>::build_args(req_args, opt_args);

	// ------------------------------------------------------------------------------------------------------- encoder
	opt_args[{"enc-type"}][2] += ", RSC";

	opt_args[{"enc-no-buff"}] =
		{"",
		 "disable the buffered encoding."};
}

template <typename B>
void Factory_encoder_RSC<B>
::store_args(const Arguments_reader& ar, encoder_parameters_RSC &params)
{
	params.type = "RSC";

	Factory_encoder_common<B>::store_args(ar, params);

	// ------------------------------------------------------------------------------------------------------- encoder
	if(ar.exist_arg({"enc-no-buff"})) params.buffered = false;
}

template <typename B>
void Factory_encoder_RSC<B>
::group_args(Arguments_reader::arg_grp& ar)
{
	Factory_encoder_common<B>::group_args(ar);
}

template <typename B>
void Factory_encoder_RSC<B>
::header(Header::params_list& head_enc, const encoder_parameters_RSC& params)
{
	Factory_encoder_common<B>::header(head_enc, params);

	// ------------------------------------------------------------------------------------------------------- encoder
	head_enc.push_back(std::make_pair("Buffered", (params.buffered ? "on" : "off")));
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct aff3ct::tools::Factory_encoder_RSC<B_8>;
template struct aff3ct::tools::Factory_encoder_RSC<B_16>;
template struct aff3ct::tools::Factory_encoder_RSC<B_32>;
template struct aff3ct::tools::Factory_encoder_RSC<B_64>;
#else
template struct aff3ct::tools::Factory_encoder_RSC<B>;
#endif
// ==================================================================================== explicit template instantiation
