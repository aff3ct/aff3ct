#include "Tools/Exception/exception.hpp"

#include "Module/Encoder/Turbo/Encoder_turbo.hpp"
#include "Module/Encoder/Turbo/Encoder_turbo_legacy.hpp"

#include "Factory_encoder_turbo.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B>
Encoder<B>* Factory_encoder_turbo
::build(const parameters       &params,
        const Interleaver<int> &itl,
              Encoder_sys<B>   *enc_n,
              Encoder_sys<B>   *enc_i)
{
	enc_i = (enc_i == nullptr) ? enc_n : enc_i;

	if (params.buffered)
	{
		if (params.type == "TURBO") return new Encoder_turbo       <B>(params.K, params.N_cw, itl, *enc_n, *enc_i, params.n_frames);
	}
	else if (enc_n == enc_i)
	{
		if (params.type == "TURBO") return new Encoder_turbo_legacy<B>(params.K, params.N_cw, itl, *enc_n,         params.n_frames);
	}

	throw cannot_allocate(__FILE__, __LINE__, __func__);
}

void Factory_encoder_turbo
::build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args)
{
	Factory_encoder::build_args(req_args, opt_args);
	Factory_interleaver::build_args(req_args, opt_args);

	opt_args[{"enc-type"}][2] += ", TURBO";

	opt_args[{"enc-no-buff"}] =
		{"",
		 "disable the buffered encoding."};

	opt_args[{"enc-poly"}] =
		{"string",
		 "the polynomials describing RSC code, should be of the form \"{A,B}\"."};

	opt_args[{"enc-json-path"}] =
		{"string",
		 "path to store the encoder and decoder traces formated in JSON."};

	opt_args[{"enc-std"}] =
		{"string",
		 "select a standard and set automatically some parameters (overwritten with user given arguments).",
		 "LTE, CCSDS"};
}

void Factory_encoder_turbo
::store_args(const Arguments_reader& ar, parameters &params)
{
	params.type = "TURBO";

	Factory_encoder::store_args(ar, params);

	if(ar.exist_arg({"enc-no-buff"})) params.buffered = false;
	if(ar.exist_arg({"enc-std"})) params.standard = ar.get_arg({"enc-std"});
	if(ar.exist_arg({"enc-json-path"})) params.json_path = ar.get_arg({"enc-json-path"});
	if (!params.json_path.empty())
		params.type = "TURBO_JSON";

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

	if (ar.exist_arg({"enc-poly"}))
	{
		auto poly_str = ar.get_arg({"enc-poly"});

#ifdef _MSC_VER
		sscanf_s   (poly_str.c_str(), "{%o,%o}", &params.poly[0], &params.poly[1]);
#else
		std::sscanf(poly_str.c_str(), "{%o,%o}", &params.poly[0], &params.poly[1]);
#endif
	}

	params.tail_length = (int)(4 * std::floor(std::log2((float)std::max(params.poly[0], params.poly[1]))));
	params.N_cw       += params.tail_length;

	params.itl.size = params.K;
	params.itl.n_frames = params.n_frames;
	Factory_interleaver::store_args(ar, params.itl);
}

void Factory_encoder_turbo
::group_args(Arguments_reader::arg_grp& ar)
{
	Factory_encoder    ::group_args(ar);
	Factory_interleaver::group_args(ar);
}

void Factory_encoder_turbo
::header(params_list& head_enc, params_list& head_itl, const parameters& params)
{
	Factory_encoder    ::header(head_enc, params);
	Factory_interleaver::header(head_itl, params.itl);

	head_enc.push_back(std::make_pair("Buffered", (params.buffered ? "on" : "off")));
	if (!params.standard.empty())
		head_enc.push_back(std::make_pair("Standard", params.standard));

	std::stringstream poly;
	poly << "{0" << std::oct << params.poly[0] << ",0" << std::oct << params.poly[1] << "}";
	head_enc.push_back(std::make_pair(std::string("Polynomials"), poly.str()));

	if (!params.json_path.empty())
		head_enc.push_back(std::make_pair("Path to the JSON file", params.json_path));
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Encoder<B_8 >* aff3ct::tools::Factory_encoder_turbo::build<B_8 >(const aff3ct::tools::Factory_encoder_turbo::parameters&, const Interleaver<int>&, Encoder_sys<B_8 >*, Encoder_sys<B_8 >*);
template aff3ct::module::Encoder<B_16>* aff3ct::tools::Factory_encoder_turbo::build<B_16>(const aff3ct::tools::Factory_encoder_turbo::parameters&, const Interleaver<int>&, Encoder_sys<B_16>*, Encoder_sys<B_16>*);
template aff3ct::module::Encoder<B_32>* aff3ct::tools::Factory_encoder_turbo::build<B_32>(const aff3ct::tools::Factory_encoder_turbo::parameters&, const Interleaver<int>&, Encoder_sys<B_32>*, Encoder_sys<B_32>*);
template aff3ct::module::Encoder<B_64>* aff3ct::tools::Factory_encoder_turbo::build<B_64>(const aff3ct::tools::Factory_encoder_turbo::parameters&, const Interleaver<int>&, Encoder_sys<B_64>*, Encoder_sys<B_64>*);
#else
template aff3ct::module::Encoder<B>* aff3ct::tools::Factory_encoder_turbo::build<B>(const aff3ct::tools::Factory_encoder_turbo::parameters&, const Interleaver<int>&, Encoder_sys<B>*, Encoder_sys<B>*);
#endif
// ==================================================================================== explicit template instantiation
