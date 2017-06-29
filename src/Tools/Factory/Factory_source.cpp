#include "Tools/Exception/exception.hpp"

#include "Module/Source/AZCW/Source_AZCW.hpp"
#include "Module/Source/Random/Source_random.hpp"
#include "Module/Source/Random/Source_random_fast.hpp"
#include "Module/Source/User/Source_user.hpp"

#include "Factory_source.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B>
Source<B>* Factory_source<B>
::build(const std::string type,
        const int         K,
        const std::string path,
        const int         seed,
        const int         n_frames)
{
	     if (type == "RAND"     ) return new Source_random     <B>(K, seed, n_frames);
	else if (type == "RAND_FAST") return new Source_random_fast<B>(K, seed, n_frames);
	else if (type == "AZCW"     ) return new Source_AZCW       <B>(K,       n_frames);
	else if (type == "USER"     ) return new Source_user       <B>(K, path, n_frames);

	throw cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B>
void Factory_source<B>
::build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args)
{
	opt_args[{"src-type"}] =
		{"string",
		 "method used to generate the codewords.",
		 "RAND, RAND_FAST, AZCW, USER"};

	opt_args[{"src-path"}] =
		{"string",
		 "path to a file containing one or a set of pre-computed source bits, to use with \"--src-type USER\"."};
}

template <typename B>
void Factory_source<B>
::store_args(const Arguments_reader& ar, source_parameters &params)
{
	// -------------------------------------------------------------------------------------------------------- source
	if(ar.exist_arg({"src-type"})) params.type = ar.get_arg({"src-type"});

	if (params.type == "AZCW")
		params.azcw = true;

	if(ar.exist_arg({"src-path"})) params.path = ar.get_arg({"src-path"});
}

template <typename B>
void Factory_source<B>
::group_args(Arguments_reader::arg_grp& ar)
{
	ar.push_back({"src", "Source parameter(s)"});
}

template <typename B>
void Factory_source<B>
::header(Header::params_list& head_src, const source_parameters& params)
{
	// -------------------------------------------------------------------------------------------------------- source
	head_src.push_back(std::make_pair("Type", params.type));

	if (params.type == "USER")
		head_src.push_back(std::make_pair("Path", params.path));
}
// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct aff3ct::tools::Factory_source<B_8>;
template struct aff3ct::tools::Factory_source<B_16>;
template struct aff3ct::tools::Factory_source<B_32>;
template struct aff3ct::tools::Factory_source<B_64>;
#else
template struct aff3ct::tools::Factory_source<B>;
#endif
// ==================================================================================== explicit template instantiation
