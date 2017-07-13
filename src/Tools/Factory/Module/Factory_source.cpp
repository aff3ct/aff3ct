#include "Tools/Exception/exception.hpp"

#include "Module/Source/AZCW/Source_AZCW.hpp"
#include "Module/Source/Random/Source_random.hpp"
#include "Module/Source/Random/Source_random_fast.hpp"
#include "Module/Source/User/Source_user.hpp"

#include "Factory_source.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B>
Source<B>* Factory_source
::build(const parameters& params)
{
	     if (params.type == "RAND"     ) return new Source_random     <B>(params.K, params.seed, params.n_frames);
	else if (params.type == "RAND_FAST") return new Source_random_fast<B>(params.K, params.seed, params.n_frames);
	else if (params.type == "AZCW"     ) return new Source_AZCW       <B>(params.K,              params.n_frames);
	else if (params.type == "USER"     ) return new Source_user       <B>(params.K, params.path, params.n_frames);

	throw cannot_allocate(__FILE__, __LINE__, __func__);
}

void Factory_source
::build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args)
{
	req_args[{"src-info-bits", "K"}] =
		{"positive_int",
		 "number of generated bits (information bits)."};

	opt_args[{"src-type"}] =
		{"string",
		 "method used to generate the codewords.",
		 "RAND, RAND_FAST, AZCW, USER"};

	opt_args[{"src-path"}] =
		{"string",
		 "path to a file containing one or a set of pre-computed source bits, to use with \"--src-type USER\"."};

	opt_args[{"src-fra", "F"}] =
		{"positive_int",
		 "set the number of inter frame level to process."};

	opt_args[{"src-seed", "S"}] =
		{"positive_int",
		 "seed used to initialize the pseudo random generators."};
}

void Factory_source
::store_args(const Arguments_reader& ar, parameters &params)
{
	params.K = ar.get_arg_int({"src-info-bits", "K"});
	if(ar.exist_arg({"src-type"})) params.type = ar.get_arg({"src-type"});
	if(ar.exist_arg({"src-path"})) params.path = ar.get_arg({"src-path"});
	if(ar.exist_arg({"src-fra", "F"})) params.n_frames = ar.get_arg_int({"src-fra", "F"});
	if(ar.exist_arg({"src-seed", "S"})) params.seed = ar.get_arg_int({"src-seed", "S"});

	if (params.type == "AZCW")
		params.azcw = true;
}

void Factory_source
::group_args(Arguments_reader::arg_grp& ar)
{
	ar.push_back({"src", "Source parameter(s)"});
}

void Factory_source
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
template aff3ct::module::Source<B_8 >* aff3ct::tools::Factory_source::build<B_8 >(const aff3ct::tools::Factory_source::parameters&);
template aff3ct::module::Source<B_16>* aff3ct::tools::Factory_source::build<B_16>(const aff3ct::tools::Factory_source::parameters&);
template aff3ct::module::Source<B_32>* aff3ct::tools::Factory_source::build<B_32>(const aff3ct::tools::Factory_source::parameters&);
template aff3ct::module::Source<B_64>* aff3ct::tools::Factory_source::build<B_64>(const aff3ct::tools::Factory_source::parameters&);
#else
template aff3ct::module::Source<B>* aff3ct::tools::Factory_source::build<B>(const aff3ct::tools::Factory_source::parameters&);
#endif
// ==================================================================================== explicit template instantiation
