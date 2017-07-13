#include "Tools/Exception/exception.hpp"

#include "Module/Encoder/NO/Encoder_NO.hpp"
#include "Module/Encoder/AZCW/Encoder_AZCW.hpp"
#include "Module/Encoder/Coset/Encoder_coset.hpp"
#include "Module/Encoder/User/Encoder_user.hpp"

#include "Factory_encoder.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B>
Encoder<B>* Factory_encoder
::build(const parameters &params)
{
	     if (params.type == "NO"   ) return new Encoder_NO   <B>(params.K,                           params.n_frames);
	else if (params.type == "AZCW" ) return new Encoder_AZCW <B>(params.K, params.N_cw,              params.n_frames);
	else if (params.type == "COSET") return new Encoder_coset<B>(params.K, params.N_cw, params.seed, params.n_frames);
	else if (params.type == "USER" ) return new Encoder_user <B>(params.K, params.N_cw, params.path, params.n_frames);

	throw cannot_allocate(__FILE__, __LINE__, __func__);
}

void Factory_encoder
::build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args)
{
	req_args[{"enc-info-bits", "K"}] =
		{"positive_int",
		 "useful number of bit transmitted (information bits)."};

	req_args[{"enc-cw-size", "N"}] =
		{"positive_int",
		 "the codeword size."};

	opt_args[{"enc-type"}] =
		{"string",
		 "select the type of encoder you want to use.",
		 "NO, COSET, USER"};

	opt_args[{"enc-path"}] =
		{"string",
		 "path to a file containing one or a set of pre-computed codewords, to use with \"--enc-type USER\"."};

	opt_args[{"enc-no-sys"}] =
		{"",
		 "disable the systematic encoding."};

	opt_args[{"enc-coset", "c"}] =
		{"",
		 "enable the coset approach."};

	opt_args[{"enc-seed", "S"}] =
		{"positive_int",
		 "seed used to initialize the pseudo random generators."};
}

void Factory_encoder
::store_args(const Arguments_reader& ar, parameters &params)
{
	params.K    = ar.get_arg_int({"enc-info-bits", "K"});
	params.N_cw = ar.get_arg_int({"enc-cw-size", "N"});
	if(ar.exist_arg({"enc-fra", "F"})) params.n_frames = ar.get_arg_int({"enc-fra", "F"});
	if(ar.exist_arg({"enc-type"})) params.type = ar.get_arg({"enc-type"});
	if(ar.exist_arg({"enc-path"})) params.path = ar.get_arg({"enc-path"});
	if(ar.exist_arg({"enc-no-sys"})) params.systematic = false;
	if(ar.exist_arg({"enc-coset", "c"})) params.coset = true;
	if(ar.exist_arg({"enc-seed", "S"})) params.seed = ar.get_arg_int({"enc-seed", "S"});

	if (params.type == "COSET")
		params.coset = true;

//	if (params.type == "AZCW")
//		params.source.type = "AZCW";
//	if (params.type == "USER")
//		params.source.type = "USER";
}

void Factory_encoder
::group_args(Arguments_reader::arg_grp& ar)
{
	ar.push_back({"enc", "Encoder parameter(s)"});
}

void Factory_encoder
::header(params_list& head_enc, const parameters& params)
{
	head_enc.push_back(std::make_pair("Type", params.type));
	head_enc.push_back(std::make_pair("Info. bits (K)", std::to_string(params.K)));
	head_enc.push_back(std::make_pair("Codeword size (N)", std::to_string(params.N_cw)));
	head_enc.push_back(std::make_pair("Inter frame level", std::to_string(params.n_frames)));
	head_enc.push_back(std::make_pair("Systematic encoding", ((params.systematic) ? "on" : "off")));
	if (params.type == "USER")
		head_enc.push_back(std::make_pair("Path", params.path));
	if (params.type == "COSET")
		head_enc.push_back(std::make_pair("Seed", std::to_string(params.seed)));

}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Encoder<B_8 >* aff3ct::tools::Factory_encoder::build<B_8 >(const aff3ct::tools::Factory_encoder::parameters&);
template aff3ct::module::Encoder<B_16>* aff3ct::tools::Factory_encoder::build<B_16>(const aff3ct::tools::Factory_encoder::parameters&);
template aff3ct::module::Encoder<B_32>* aff3ct::tools::Factory_encoder::build<B_32>(const aff3ct::tools::Factory_encoder::parameters&);
template aff3ct::module::Encoder<B_64>* aff3ct::tools::Factory_encoder::build<B_64>(const aff3ct::tools::Factory_encoder::parameters&);
#else
template aff3ct::module::Encoder<B>* aff3ct::tools::Factory_encoder::build<B>(const aff3ct::tools::Factory_encoder::parameters&);
#endif
// ==================================================================================== explicit template instantiation
