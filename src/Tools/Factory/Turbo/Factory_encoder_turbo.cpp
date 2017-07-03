#include "Tools/Exception/exception.hpp"

#include "Module/Encoder/Turbo/Encoder_turbo.hpp"
#include "Module/Encoder/Turbo/Encoder_turbo_legacy.hpp"

#include "Factory_encoder_turbo.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B>
Encoder<B>* Factory_encoder_turbo<B>
::build(const encoder_parameters_turbo &params,
        const Interleaver<int> &itl,
              Encoder_sys<B>   *enc_n,
              Encoder_sys<B>   *enc_i)
{
	enc_i = (enc_i == nullptr) ? enc_n : enc_i;

	if (params.buffered)
	{
		if (params.type == "TURBO") return new Encoder_turbo       <B>(params.K, params.N, itl, *enc_n, *enc_i, params.n_frames);
	}
	else if (enc_n == enc_i)
	{
		if (params.type == "TURBO") return new Encoder_turbo_legacy<B>(params.K, params.N, itl, *enc_n,         params.n_frames);
	}

	throw cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B>
void Factory_encoder_turbo<B>
::build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args)
{
	Factory_encoder_common<B>::build_args(req_args, opt_args);

	// --------------------------------------------------------------------------------------------------- interleaver
	Factory_interleaver<int>::build_args(req_args, opt_args);

	// ------------------------------------------------------------------------------------------------------- encoder
	opt_args[{"enc-type"}][2] += ", TURBO";

	opt_args[{"enc-no-buff"}] =
		{"",
		 "disable the buffered encoding."};

	// ---------------------------------------------------------------------------------------------------------- code
	opt_args[{"cde-poly"}] =
		{"string",
		 "the polynomials describing RSC code, should be of the form \"{A,B}\"."};

	opt_args[{"cde-json-path"}] =
		{"string",
		 "path to store the encoder and decoder traces formated in JSON."};

	opt_args[{"cde-std"}] =
		{"string",
		 "select a standard and set automatically some parameters (overwritten with user given arguments).",
		 "LTE, CCSDS"};
}

template <typename B>
void Factory_encoder_turbo<B>
::store_args(const Arguments_reader& ar, encoder_parameters_turbo &params,
             const int K, const int N, const int n_frames)
{
	params.type = "TURBO";

	Factory_encoder_common<B>::store_args(ar, params, K, N, n_frames);

	// ------------------------------------------------------------------------------------------------------- encoder
	if(ar.exist_arg({"enc-no-buff"})) params.buffered = false;


	// --------------------------------------------------------------------------------------------------- interleaver
	Factory_interleaver<int>::store_args(ar, params.itl, K, n_frames);


	// ---------------------------------------------------------------------------------------------------------- code
	if(ar.exist_arg({"cde-std"})) params.standard = this->ar.get_arg({"cde-std"});

	if(ar.exist_arg({"cde-json-path"})) params.json_path = this->ar.get_arg({"cde-json-path"});
	if (!params.json_path.empty())
		params.encoder.type = "TURBO_JSON";

	if (params.standard == "LTE")
	{
		params.poly = {013, 015};
		if (!ar.exist_arg({"itl-type"}))
			params.itl.type = "LTE";
	}

	if (params.type == "CCSDS")
	{
		params.poly = {023, 033};
		if (!ar.exist_arg({"itl-type"}))
			params.itl.type = "CCSDS";
	}

	if (ar.exist_arg({"cde-poly"}))
	{
		auto poly_str = ar.get_arg({"cde-poly"});

#ifdef _MSC_VER
		sscanf_s   (poly_str.c_str(), "{%o,%o}", &params.poly[0], &params.poly[1]);
#else
		std::sscanf(poly_str.c_str(), "{%o,%o}", &params.poly[0], &params.poly[1]);
#endif
	}

	params.tail_length = (int)(4 * std::floor(std::log2((float)std::max(params.poly[0], params.poly[1]))));
	params.N_pct       =     params.N + params.tail_length;
	params.N           = 3 * params.K + params.tail_length;
}

template <typename B>
void Factory_encoder_turbo<B>
::group_args(Arguments_reader::arg_grp& ar)
{
	Factory_encoder_common<B>::group_args(ar);

	// --------------------------------------------------------------------------------------------------- interleaver
	Factory_interleaver<int>::group_args(ar);
}

template <typename B>
void Factory_encoder_turbo<B>
::header(Header::params_list& head_enc, Header::params_list& head_cde, const encoder_parameters_turbo& params)
{
	Factory_encoder_common<B>::header(head_enc, params);

	// ------------------------------------------------------------------------------------------------------- encoder
	head_enc.push_back(std::make_pair("Buffered", (params.buffered ? "on" : "off")));

	// ---------------------------------------------------------------------------------------------------------- code
	if (!params.standard.empty())
		head_cde.push_back(std::make_pair("Turbo standard", params.standard));

	std::stringstream poly;
	poly << "{0" << std::oct << params.poly[0] << ",0" << std::oct << params.poly[1] << "}";
	head_cde.push_back(std::make_pair(std::string("Polynomials"), poly.str()));

	if (!params.json_path.empty())
		head_cde.push_back(std::make_pair("Path to the JSON file", params.json_path));
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct aff3ct::tools::Factory_encoder_turbo<B_8>;
template struct aff3ct::tools::Factory_encoder_turbo<B_16>;
template struct aff3ct::tools::Factory_encoder_turbo<B_32>;
template struct aff3ct::tools::Factory_encoder_turbo<B_64>;
#else
template struct aff3ct::tools::Factory_encoder_turbo<B>;
#endif
// ==================================================================================== explicit template instantiation
