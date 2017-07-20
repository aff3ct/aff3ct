#include "Tools/Exception/exception.hpp"

#include "Module/Source/AZCW/Source_AZCW.hpp"
#include "Module/Source/Random/Source_random.hpp"
#include "Module/Source/Random/Source_random_fast.hpp"
#include "Module/Source/User/Source_user.hpp"

#include "Source.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

template <typename B>
module::Source<B>* Source
::build(const parameters& params)
{
	     if (params.type == "RAND"     ) return new module::Source_random     <B>(params.K, params.seed, params.n_frames);
	else if (params.type == "RAND_FAST") return new module::Source_random_fast<B>(params.K, params.seed, params.n_frames);
	else if (params.type == "AZCW"     ) return new module::Source_AZCW       <B>(params.K,              params.n_frames);
	else if (params.type == "USER"     ) return new module::Source_user       <B>(params.K, params.path, params.n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

void Source
::build_args(arg_map &req_args, arg_map &opt_args)
{
	req_args[{"src-info-bits", "K"}] =
		{"positive_int",
		 "number of generated bits (information bits)."};

	opt_args[{"src-fra", "F"}] =
		{"positive_int",
		 "set the number of inter frame level to process."};

	opt_args[{"src-type"}] =
		{"string",
		 "method used to generate the codewords.",
		 "RAND, RAND_FAST, AZCW, USER"};

	opt_args[{"src-path"}] =
		{"string",
		 "path to a file containing one or a set of pre-computed source bits, to use with \"--src-type USER\"."};

	opt_args[{"src-seed", "S"}] =
		{"positive_int",
		 "seed used to initialize the pseudo random generators."};
}

void Source
::store_args(const tools::Arguments_reader& ar, parameters &params)
{
	if(ar.exist_arg({"src-info-bits", "K"})) params.K        = ar.get_arg_int({"src-info-bits", "K"});
	if(ar.exist_arg({"src-fra",       "F"})) params.n_frames = ar.get_arg_int({"src-fra",       "F"});
	if(ar.exist_arg({"src-type"          })) params.type     = ar.get_arg    ({"src-type"          });
	if(ar.exist_arg({"src-path"          })) params.path     = ar.get_arg    ({"src-path"          });
	if(ar.exist_arg({"src-seed",      "S"})) params.seed     = ar.get_arg_int({"src-seed",      "S"});
}

void Source
::group_args(arg_grp& ar)
{
	ar.push_back({"src", "Source parameter(s)"});
}

void Source
::header(params_list& head_src, const parameters& params)
{
	head_src.push_back(std::make_pair("Type", params.type));
	head_src.push_back(std::make_pair("Info. bits (K)", std::to_string(params.K)));
	head_src.push_back(std::make_pair("Inter frame level", std::to_string(params.n_frames)));
	if (params.type == "USER")
		head_src.push_back(std::make_pair("Path", params.path));
	if (params.type == "RAND" || params.type == "RAND_FAST")
		head_src.push_back(std::make_pair("Seed", std::to_string(params.seed)));
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Source<B_8 >* aff3ct::factory::Source::build<B_8 >(const aff3ct::factory::Source::parameters&);
template aff3ct::module::Source<B_16>* aff3ct::factory::Source::build<B_16>(const aff3ct::factory::Source::parameters&);
template aff3ct::module::Source<B_32>* aff3ct::factory::Source::build<B_32>(const aff3ct::factory::Source::parameters&);
template aff3ct::module::Source<B_64>* aff3ct::factory::Source::build<B_64>(const aff3ct::factory::Source::parameters&);
#else
template aff3ct::module::Source<B>* aff3ct::factory::Source::build<B>(const aff3ct::factory::Source::parameters&);
#endif
// ==================================================================================== explicit template instantiation
