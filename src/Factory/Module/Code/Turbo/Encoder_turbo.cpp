#include "Tools/Exception/exception.hpp"

#include "Module/Encoder/Turbo/Encoder_turbo.hpp"
#include "Module/Encoder/Turbo/Encoder_turbo_legacy.hpp"

#include "Encoder_turbo.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Encoder_turbo::name   = "Encoder Turbo";
const std::string aff3ct::factory::Encoder_turbo::prefix = "enc";

template <typename B>
module::Encoder<B>* Encoder_turbo
::build(const parameters               &params,
        const module::Interleaver<int> &itl,
              module::Encoder_sys<B>   *enc_n,
              module::Encoder_sys<B>   *enc_i)
{
	enc_i = (enc_i == nullptr) ? enc_n : enc_i;

	if (params.buffered)
	{
		if (params.type == "TURBO") return new module::Encoder_turbo       <B>(params.K, params.N_cw, itl, *enc_n, *enc_i, params.n_frames);
	}
	else if (enc_n == enc_i)
	{
		if (params.type == "TURBO") return new module::Encoder_turbo_legacy<B>(params.K, params.N_cw, itl, *enc_n,         params.n_frames);
	}

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

void Encoder_turbo
::build_args(arg_map &req_args, arg_map &opt_args, const std::string p)
{
	Encoder::build_args(req_args, opt_args);
	req_args.erase({p+"-cw-size", "N"});

	Interleaver::build_args(req_args, opt_args, "itl");
	req_args.erase({"itl-size"    });
	opt_args.erase({"itl-fra", "F"});

	opt_args[{p+"-type"}][2] += ", TURBO";

	opt_args[{p+"-no-buff"}] =
		{"",
		 "disable the buffered encoding."};

	opt_args[{p+"-poly"}] =
		{"string",
		 "the polynomials describing RSC code, should be of the form \"{A,B}\"."};

	opt_args[{p+"-json-path"}] =
		{"string",
		 "path to store the encoder and decoder traces formated in JSON."};

	opt_args[{p+"-std"}] =
		{"string",
		 "select a standard and set automatically some parameters (overwritten with user given arguments).",
		 "LTE, CCSDS"};
}

void Encoder_turbo
::store_args(const arg_val_map &vals, parameters &params, const std::string p)
{
	params.type = "TURBO";

	Encoder::store_args(vals, params);

	if(exist(vals, {p+"-std"      })) params.standard  = vals.at({p+"-std"      });
	if(exist(vals, {p+"-json-path"})) params.json_path = vals.at({p+"-json-path"});
	if(exist(vals, {p+"-no-buff"  })) params.buffered  = false;

	if (!params.json_path.empty())
		params.type = "TURBO_JSON";

	if (params.standard == "LTE")
	{
		params.poly = {013, 015};
		if (!exist(vals, {"itl-type"}))
			params.itl.type = "LTE";
	}

	if (params.standard == "CCSDS")
	{
		params.poly = {023, 033};
		if (!exist(vals, {"itl-type"}))
			params.itl.type = "CCSDS";
	}

	if (exist(vals, {p+"-poly"}))
	{
		auto poly_str = vals.at({p+"-poly"});

#ifdef _MSC_VER
		sscanf_s   (poly_str.c_str(), "{%o,%o}", &params.poly[0], &params.poly[1]);
#else
		std::sscanf(poly_str.c_str(), "{%o,%o}", &params.poly[0], &params.poly[1]);
#endif
	}

	params.tail_length = (int)(4 * std::floor(std::log2((float)std::max(params.poly[0], params.poly[1]))));
	params.N_cw        = 3 * params.K + params.tail_length;
	params.R           = (float)params.K / (float)params.N_cw;

	params.itl.size     = params.K;
	params.itl.n_frames = params.n_frames;
	Interleaver::store_args(vals, params.itl, "itl");
}

void Encoder_turbo
::make_header(params_list& head_enc, params_list& head_itl, const parameters& params)
{
	Encoder    ::make_header(head_enc, params);
	Interleaver::make_header(head_itl, params.itl);

	if (params.tail_length)
		head_enc.push_back(std::make_pair("Tail length", std::to_string(params.tail_length)));

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
template aff3ct::module::Encoder<B_8 >* aff3ct::factory::Encoder_turbo::build<B_8 >(const aff3ct::factory::Encoder_turbo::parameters&, const aff3ct::module::Interleaver<int>&, aff3ct::module::Encoder_sys<B_8 >*, aff3ct::module::Encoder_sys<B_8 >*);
template aff3ct::module::Encoder<B_16>* aff3ct::factory::Encoder_turbo::build<B_16>(const aff3ct::factory::Encoder_turbo::parameters&, const aff3ct::module::Interleaver<int>&, aff3ct::module::Encoder_sys<B_16>*, aff3ct::module::Encoder_sys<B_16>*);
template aff3ct::module::Encoder<B_32>* aff3ct::factory::Encoder_turbo::build<B_32>(const aff3ct::factory::Encoder_turbo::parameters&, const aff3ct::module::Interleaver<int>&, aff3ct::module::Encoder_sys<B_32>*, aff3ct::module::Encoder_sys<B_32>*);
template aff3ct::module::Encoder<B_64>* aff3ct::factory::Encoder_turbo::build<B_64>(const aff3ct::factory::Encoder_turbo::parameters&, const aff3ct::module::Interleaver<int>&, aff3ct::module::Encoder_sys<B_64>*, aff3ct::module::Encoder_sys<B_64>*);
#else
template aff3ct::module::Encoder<B>* aff3ct::factory::Encoder_turbo::build<B>(const aff3ct::factory::Encoder_turbo::parameters&, const aff3ct::module::Interleaver<int>&, aff3ct::module::Encoder_sys<B>*, aff3ct::module::Encoder_sys<B>*);
#endif
// ==================================================================================== explicit template instantiation
