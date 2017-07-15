#include "Factory_decoder.hpp"

using namespace aff3ct;
using namespace tools;

void Factory_decoder::build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args)
{
	req_args[{"dec-info-bits", "K"}] =
		{"positive_int",
		 "useful number of bit transmitted (information bits)."};

	req_args[{"dec-cw-size", "N"}] =
		{"positive_int",
		 "the codeword size."};

	opt_args[{"dec-fra", "F"}] =
		{"positive_int",
		 "set the number of inter frame level to process."};

	opt_args[{"dec-type", "D"}] =
		{"string",
		 "select the algorithm you want to decode the codeword."};

	opt_args[{"dec-implem"}] =
		{"string",
		 "select the implementation of the algorithm to decode."};
}

void Factory_decoder::store_args(const Arguments_reader& ar, parameters &params)
{
	params.K = ar.get_arg_int({"dec-info-bits", "K"});
	params.N_cw = ar.get_arg_int({"dec-cw-size", "N"});
	params.R = (float)params.K / (float)params.N_cw;
	if(ar.exist_arg({"dec-fra", "F"})) params.n_frames = ar.get_arg_int({"dec-fra", "F"});
	if(ar.exist_arg({"dec-type", "D"})) params.type = ar.get_arg({"dec-type", "D"});
	if(ar.exist_arg({"dec-implem"})) params.implem = ar.get_arg({"dec-implem"});
	if(ar.exist_arg({"enc-no-sys"})) params.systematic = false;
}

void Factory_decoder::group_args(Arguments_reader::arg_grp& ar)
{
	ar.push_back({"dec", "Decoder parameter(s)"});
}

void Factory_decoder::header(params_list& head_dec, const parameters& params)
{
	head_dec.push_back(std::make_pair("Type (D)",params.type));
	if(params.implem.size())
		head_dec.push_back(std::make_pair("Implementation", params.implem));
	head_dec.push_back(std::make_pair("Info. bits (K)", std::to_string(params.K)));
	head_dec.push_back(std::make_pair("Codeword size (N)", std::to_string(params.N_cw)));
	head_dec.push_back(std::make_pair("Systematic", ((params.systematic) ? "on" : "off")));
	head_dec.push_back(std::make_pair("Inter frame level", std::to_string(params.n_frames)));
}
