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
::build_args(tools::Arguments_reader::arg_map &req_args, tools::Arguments_reader::arg_map &opt_args)
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
::store_args(const tools::Arguments_reader& ar, tools::parameters &params)
{
	// -------------------------------------------------------------------------------------------- default parameters
	params.source     .type              = "RAND";
	params.source     .path              = "";

	// -------------------------------------------------------------------------------------------------------- source
	if(ar.exist_arg({"src-type"})) params.source.type = ar.get_arg({"src-type"});

	if (params.source.type == "AZCW")
		params.code.azcw = true;

	if(ar.exist_arg({"src-path"})) params.source.path = ar.get_arg({"src-path"});
}

template <typename B>
void Factory_source<B>
::group_args(tools::Arguments_reader::arg_grp& ar)
{
	ar.push_back({"src",  "Source parameter(s)"     });
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
