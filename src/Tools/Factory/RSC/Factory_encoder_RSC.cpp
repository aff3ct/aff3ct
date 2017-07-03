#include "Tools/Exception/exception.hpp"

#include "Module/Encoder/RSC/Encoder_RSC_generic_sys.hpp"
#include "Module/Encoder/RSC/Encoder_RSC_generic_json_sys.hpp"

#include "Factory_encoder_RSC.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B>
Encoder_RSC_sys<B>* Factory_encoder_RSC<B>
::build(const encoder_parameters_RSC &params, std::ostream &stream)
{
	     if (params.type == "RSC_JSON") return new Encoder_RSC_generic_json_sys<B>(params.K, params.N, params.buffered, params.poly, stream, params.n_frames);
	else if (params.type == "RSC"     ) return new Encoder_RSC_generic_sys     <B>(params.K, params.N, params.buffered, params.poly,         params.n_frames);

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
::store_args(const Arguments_reader& ar, encoder_parameters_RSC &params,
             const int K, const int N, const int n_frames)
{
	params.type = "RSC";

	Factory_encoder_common<B>::store_args(ar, params);

	// ------------------------------------------------------------------------------------------------------- encoder
	if(ar.exist_arg({"enc-no-buff"})) params.buffered = false;


	// ---------------------------------------------------------------------------------------------------------- code
	if (ar.exist_arg({"cde-poly"}))
	{
		auto poly_str = ar.get_arg({"cde-poly"});

#ifdef _MSC_VER
		sscanf_s   (poly_str.c_str(), "{%o,%o}", &params.poly[0], &params.poly[1]);
#else
		std::sscanf(poly_str.c_str(), "{%o,%o}", &params.poly[0], &params.poly[1]);
#endif
	}

	if (params.type == "LTE")
	{
		params.type = "BCJR";
		params.poly = {013, 015};
	}

	if (params.type == "CCSDS")
	{
		params.type = "BCJR";
		params.poly = {023, 033};
	}

	if (!(params.poly[0] == 013 && params.poly[1] == 015)) // if not LTE BCJR
	{
		params.type          = "BCJR";
		params.implem        = "GENERIC";
		params.simd_strategy = "";
	}

	params.tail_length = (int)(2 * std::floor(std::log2((float)std::max(params.poly[0], params.poly[1]))));
	params.N += params.tail_length;
}

template <typename B>
void Factory_encoder_RSC<B>
::group_args(Arguments_reader::arg_grp& ar)
{
	Factory_encoder_common<B>::group_args(ar);
}

template <typename B>
void Factory_encoder_RSC<B>
::header(Header::params_list& head_enc, Header::params_list& head_cde, const encoder_parameters_RSC& params)
{
	Factory_encoder_common<B>::header(head_enc, params);

	// ------------------------------------------------------------------------------------------------------- encoder
	head_enc.push_back(std::make_pair("Buffered", (params.buffered ? "on" : "off")));

	// ---------------------------------------------------------------------------------------------------------- code
	std::stringstream poly;
	poly << "{0" << std::oct << params.poly[0] << ",0" << std::oct << params.poly[1] << "}";
	head_cde.push_back(std::make_pair(std::string("Polynomials"), poly.str()));
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
