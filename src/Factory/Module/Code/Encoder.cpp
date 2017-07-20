#include "Tools/Exception/exception.hpp"

#include "Module/Encoder/NO/Encoder_NO.hpp"
#include "Module/Encoder/AZCW/Encoder_AZCW.hpp"
#include "Module/Encoder/Coset/Encoder_coset.hpp"
#include "Module/Encoder/User/Encoder_user.hpp"

#include "Encoder.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

template <typename B>
module::Encoder<B>* Encoder
::build(const parameters &params)
{
	     if (params.type == "NO"   ) return new module::Encoder_NO   <B>(params.K,                           params.n_frames);
	else if (params.type == "AZCW" ) return new module::Encoder_AZCW <B>(params.K, params.N_cw,              params.n_frames);
	else if (params.type == "COSET") return new module::Encoder_coset<B>(params.K, params.N_cw, params.seed, params.n_frames);
	else if (params.type == "USER" ) return new module::Encoder_user <B>(params.K, params.N_cw, params.path, params.n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

void Encoder
::build_args(arg_map &req_args, arg_map &opt_args)
{
	req_args[{"enc-info-bits", "K"}] =
		{"positive_int",
		 "useful number of bit transmitted (information bits)."};

	req_args[{"enc-cw-size", "N"}] =
		{"positive_int",
		 "the codeword size."};

	opt_args[{"enc-fra", "F"}] =
		{"positive_int",
		 "set the number of inter frame level to process."};

	opt_args[{"enc-type"}] =
		{"string",
		 "select the type of encoder you want to use.",
		 "NO, AZCW, COSET, USER"};

	opt_args[{"enc-path"}] =
		{"string",
		 "path to a file containing one or a set of pre-computed codewords, to use with \"--enc-type USER\"."};

	opt_args[{"enc-seed", "S"}] =
		{"positive_int",
		 "seed used to initialize the pseudo random generators."};
}

void Encoder
::store_args(const tools::Arguments_reader& ar, parameters &params)
{
	if(ar.exist_arg({"enc-info-bits", "K"})) params.K          = ar.get_arg_int({"enc-info-bits", "K"});
	if(ar.exist_arg({"enc-cw-size",   "N"})) params.N_cw       = ar.get_arg_int({"enc-cw-size",   "N"});
	if(ar.exist_arg({"enc-fra",       "F"})) params.n_frames   = ar.get_arg_int({"enc-fra",       "F"});
	if(ar.exist_arg({"enc-seed",      "S"})) params.seed       = ar.get_arg_int({"enc-seed",      "S"});
	if(ar.exist_arg({"enc-type"          })) params.type       = ar.get_arg    ({"enc-type"          });
	if(ar.exist_arg({"enc-path"          })) params.path       = ar.get_arg    ({"enc-path"          });
	if(ar.exist_arg({"enc-no-sys"        })) params.systematic = false;

	params.R = (float)params.K / (float)params.N_cw;
}

void Encoder
::group_args(arg_grp& ar)
{
	ar.push_back({"enc", "Encoder parameter(s)"});
}

void Encoder
::header(params_list& head_enc, const parameters& params)
{
	head_enc.push_back(std::make_pair("Type", params.type));
	head_enc.push_back(std::make_pair("Info. bits (K)", std::to_string(params.K)));
	head_enc.push_back(std::make_pair("Codeword size (N)", std::to_string(params.N_cw)));
	head_enc.push_back(std::make_pair("Code rate (R)", std::to_string(params.R)));
	head_enc.push_back(std::make_pair("Inter frame level", std::to_string(params.n_frames)));
	head_enc.push_back(std::make_pair("Systematic", ((params.systematic) ? "yes" : "no")));
	if (params.type == "USER")
		head_enc.push_back(std::make_pair("Path", params.path));
	if (params.type == "COSET")
		head_enc.push_back(std::make_pair("Seed", std::to_string(params.seed)));

}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Encoder<B_8 >* aff3ct::factory::Encoder::build<B_8 >(const aff3ct::factory::Encoder::parameters&);
template aff3ct::module::Encoder<B_16>* aff3ct::factory::Encoder::build<B_16>(const aff3ct::factory::Encoder::parameters&);
template aff3ct::module::Encoder<B_32>* aff3ct::factory::Encoder::build<B_32>(const aff3ct::factory::Encoder::parameters&);
template aff3ct::module::Encoder<B_64>* aff3ct::factory::Encoder::build<B_64>(const aff3ct::factory::Encoder::parameters&);
#else
template aff3ct::module::Encoder<B>* aff3ct::factory::Encoder::build<B>(const aff3ct::factory::Encoder::parameters&);
#endif
// ==================================================================================== explicit template instantiation
