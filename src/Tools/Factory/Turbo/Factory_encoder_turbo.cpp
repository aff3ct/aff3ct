#include "Tools/Exception/exception.hpp"

#include "Module/Encoder/Turbo/Encoder_turbo.hpp"
#include "Module/Encoder/Turbo/Encoder_turbo_legacy.hpp"

#include "Factory_encoder_turbo.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B>
Encoder<B>* Factory_encoder_turbo<B>
::build(const std::string       type,
        const int               K,
        const int               N,
        const Interleaver<int> &itl,
              Encoder_sys<B>   *enc_n,
              Encoder_sys<B>   *enc_i,
        const bool              buffered,
        const int               n_frames)
{
	enc_i = (enc_i == nullptr) ? enc_n : enc_i;

	if (buffered)
	{
		if (type == "TURBO") return new Encoder_turbo       <B>(K, N, itl, *enc_n, *enc_i, n_frames);
	}
	else if (enc_n == enc_i)
	{
		if (type == "TURBO") return new Encoder_turbo_legacy<B>(K, N, itl, *enc_n,         n_frames);
	}

	throw cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B>
void Factory_encoder_turbo<B>
::build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args)
{
	Factory_encoder_common<B>::build_args(req_args, opt_args);

	// ------------------------------------------------------------------------------------------------------- encoder
	opt_args[{"enc-type"}][2] += ", TURBO";

	opt_args[{"enc-no-buff"}] =
		{"",
		 "disable the buffered encoding."};

//	opt_args[{"sim-json-path"}] =
//		{"string",
//		 "path to store the encoder and decoder traces formated in JSON."};
}

template <typename B>
void Factory_encoder_turbo<B>
::store_args(const Arguments_reader& ar, encoder_parameters_turbo &params)
{
	params.type = "TURBO";

	Factory_encoder_common<B>::store_args(ar, params);

	// ------------------------------------------------------------------------------------------------------- encoder
	if(ar.exist_arg({"enc-no-buff"})) params.buffered = false;

//	if(ar.exist_arg({"sim-json-path"})) params.json_path = this->ar.get_arg({"sim-json-path"});
//	if (!this->params.simulation.json_path.empty())
//		this->params.encoder.type = "TURBO_JSON";
}

template <typename B>
void Factory_encoder_turbo<B>
::group_args(Arguments_reader::arg_grp& ar)
{
	Factory_encoder_common<B>::group_args(ar);
}

template <typename B>
void Factory_encoder_turbo<B>
::header(Header::params_list& head_enc, const encoder_parameters_turbo& params)
{
	Factory_encoder_common<B>::header(head_enc, params);

	// ------------------------------------------------------------------------------------------------------- encoder
	head_enc.push_back(std::make_pair("Buffered", (params.buffered ? "on" : "off")));

//	if (!params.json_path.empty())
//		head_enc.push_back(std::make_pair("Path to the JSON file", params.json_path));
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
