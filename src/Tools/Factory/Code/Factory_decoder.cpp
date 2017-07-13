#include "Factory_decoder.hpp"

using namespace aff3ct;
using namespace tools;

void Factory_decoder::build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args)
{
	// ------------------------------------------------------------------------------------------------------- decoder
	opt_args[{"dec-type", "D"}] =
		{"string",
		 "select the algorithm you want to decode the codeword."};

	opt_args[{"dec-implem"}] =
		{"string",
		 "select the implementation of the algorithm to decode."};
}

void Factory_decoder::store_args(const Arguments_reader& ar, parameters &params,
                                        const int K, const int N, const int n_frames)
{
	// ------------------------------------------------------------------------------------------------------- decoder
	if(ar.exist_arg({"dec-type",  "D"})) params.type   = ar.get_arg({"dec-type",  "D"});
	if(ar.exist_arg({"dec-implem"    })) params.implem = ar.get_arg({"dec-implem"    });

	// ---------------------------------------------------------------------------------------------------------- code
	params.K = K;
	params.N = N;

	// ---------------------------------------------------------------------------------------------------------- simu
	params.n_frames = n_frames;
}

void Factory_decoder::group_args(Arguments_reader::arg_grp& ar)
{
	ar.push_back({"dec", "Decoder parameter(s)"});
}

void Factory_decoder::header(params_list& head_dec, const parameters& params)
{
	// ------------------------------------------------------------------------------------------------------- decoder
	head_dec.push_back(std::make_pair("Type (D)",params.type));

	if(params.implem.size())
		head_dec.push_back(std::make_pair("Implementation", params.implem));
}
