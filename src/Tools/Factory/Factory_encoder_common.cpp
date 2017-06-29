#include "Tools/Exception/exception.hpp"

#include "Module/Encoder/NO/Encoder_NO.hpp"
#include "Module/Encoder/AZCW/Encoder_AZCW.hpp"
#include "Module/Encoder/Coset/Encoder_coset.hpp"
#include "Module/Encoder/User/Encoder_user.hpp"

#include "Factory_encoder_common.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B>
Encoder<B>* Factory_encoder_common<B>
::build(const std::string type,
        const int         K,
        const int         N,
        const std::string path,
        const int         seed,
        const int         n_frames)
{
	     if (type == "NO"   ) return new Encoder_NO   <B>(K,          n_frames);
	else if (type == "AZCW" ) return new Encoder_AZCW <B>(K, N,       n_frames);
	else if (type == "COSET") return new Encoder_coset<B>(K, N, seed, n_frames);
	else if (type == "USER" ) return new Encoder_user <B>(K, N, path, n_frames);

	throw cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B>
void Factory_encoder_common<B>
::build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args)
{
	// ------------------------------------------------------------------------------------------------------- encoder
	opt_args[{"enc-type"}] =
		{"string",
		 "select the type of encoder you want to use.",
		 "AZCW, COSET, USER"};

	opt_args[{"enc-path"}] =
		{"string",
		 "path to a file containing one or a set of pre-computed codewords, to use with \"--enc-type USER\"."};

	opt_args[{"enc-no-sys"}] =
		{"",
		 "disable the systematic encoding."};
}

template <typename B>
void Factory_encoder_common<B>
::store_args(const Arguments_reader& ar, encoder_parameters &params)
{
	// ------------------------------------------------------------------------------------------------------- encoder
	if(ar.exist_arg({"enc-type"  })) params.type = ar.get_arg({"enc-type"});
//	if (params.type == "COSET")
//		params.code.coset = true;
//	if (params.type == "AZCW")
//		params.source.type = "AZCW";
//	if (params.type == "USER")
//		params.source.type = "USER";

	if(ar.exist_arg({"enc-path"  })) params.path = ar.get_arg({"enc-path"});

	if(ar.exist_arg({"enc-no-sys"})) params.systematic = false;
}

template <typename B>
void Factory_encoder_common<B>
::group_args(Arguments_reader::arg_grp& ar)
{
	ar.push_back({"enc", "Encoder parameter(s)"});
}

template <typename B>
void Factory_encoder_common<B>
::header(Header::params_list& head_enc, const encoder_parameters& params)
{
	// ------------------------------------------------------------------------------------------------------- encoder
	head_enc.push_back(std::make_pair("Type", params.type));

	if (params.type == "USER")
		head_enc.push_back(std::make_pair("Path", params.path));

	std::string syst_enc = ((params.systematic) ? "on" : "off");
	head_enc.push_back(std::make_pair("Systematic encoding", syst_enc));
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct aff3ct::tools::Factory_encoder_common<B_8>;
template struct aff3ct::tools::Factory_encoder_common<B_16>;
template struct aff3ct::tools::Factory_encoder_common<B_32>;
template struct aff3ct::tools::Factory_encoder_common<B_64>;
#else
template struct aff3ct::tools::Factory_encoder_common<B>;
#endif
// ==================================================================================== explicit template instantiation
