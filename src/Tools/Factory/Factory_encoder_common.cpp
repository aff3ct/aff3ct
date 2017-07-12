#include "Tools/Exception/exception.hpp"

#include "Module/Encoder/NO/Encoder_NO.hpp"
#include "Module/Encoder/AZCW/Encoder_AZCW.hpp"
#include "Module/Encoder/Coset/Encoder_coset.hpp"
#include "Module/Encoder/User/Encoder_user.hpp"

#include "Factory_encoder_common.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B>
Encoder<B>* Factory_encoder_common
::build(const parameters &params, const int seed)
{
	     if (params.type == "NO"   ) return new Encoder_NO   <B>(params.K,                        params.n_frames);
	else if (params.type == "AZCW" ) return new Encoder_AZCW <B>(params.K, params.N,              params.n_frames);
	else if (params.type == "COSET") return new Encoder_coset<B>(params.K, params.N, seed,        params.n_frames);
	else if (params.type == "USER" ) return new Encoder_user <B>(params.K, params.N, params.path, params.n_frames);

	throw cannot_allocate(__FILE__, __LINE__, __func__);
}

void Factory_encoder_common
::build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args)
{
	// ------------------------------------------------------------------------------------------------------- encoder
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

	// ---------------------------------------------------------------------------------------------------------- code
	opt_args[{"cde-coset", "c"}] =
		{"",
		 "enable the coset approach."};
}

void Factory_encoder_common
::store_args(const Arguments_reader& ar, parameters &params,
             const int K, const int N, const int n_frames)
{
	// ------------------------------------------------------------------------------------------------------- encoder
	if(ar.exist_arg({"enc-type"  })) params.type = ar.get_arg({"enc-type"});
	if (params.type == "COSET")
		params.coset = true;

//	if (params.type == "AZCW")
//		params.source.type = "AZCW";
//	if (params.type == "USER")
//		params.source.type = "USER";

	if(ar.exist_arg({"enc-path"  })) params.path = ar.get_arg({"enc-path"});

	if(ar.exist_arg({"enc-no-sys"})) params.systematic = false;

	// ---------------------------------------------------------------------------------------------------------- code
	params.K = K;
	params.N = N;

	if(ar.exist_arg({"cde-coset", "c"})) params.coset = true;

	// ---------------------------------------------------------------------------------------------------------- simu
	params.n_frames = n_frames;
}

void Factory_encoder_common
::group_args(Arguments_reader::arg_grp& ar)
{
	ar.push_back({"enc", "Encoder parameter(s)"});
}

void Factory_encoder_common
::header(Header::params_list& head_enc, Header::params_list& head_cde, const parameters& params)
{
	// ------------------------------------------------------------------------------------------------------- encoder
	head_enc.push_back(std::make_pair("Type", params.type));

	if (params.type == "USER")
		head_enc.push_back(std::make_pair("Path", params.path));

	head_enc.push_back(std::make_pair("Systematic encoding", ((params.systematic) ? "on" : "off")));

	// ---------------------------------------------------------------------------------------------------------- code
	head_cde.push_back(std::make_pair("Coset approach (c)", ((params.coset) ? "on" : "off")));

}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Encoder<B_8 >* aff3ct::tools::Factory_encoder_common::build<B_8 >(const aff3ct::tools::Factory_encoder_common::parameters&, int);
template aff3ct::module::Encoder<B_16>* aff3ct::tools::Factory_encoder_common::build<B_16>(const aff3ct::tools::Factory_encoder_common::parameters&, int);
template aff3ct::module::Encoder<B_32>* aff3ct::tools::Factory_encoder_common::build<B_32>(const aff3ct::tools::Factory_encoder_common::parameters&, int);
template aff3ct::module::Encoder<B_64>* aff3ct::tools::Factory_encoder_common::build<B_64>(const aff3ct::tools::Factory_encoder_common::parameters&, int);
#else
template aff3ct::module::Encoder<B>* aff3ct::tools::Factory_encoder_common::build<B>(const aff3ct::tools::Factory_encoder_common::parameters&, int);
#endif
// ==================================================================================== explicit template instantiation
