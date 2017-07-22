#include "Decoder.hpp"

using namespace aff3ct::factory;

const std::string aff3ct::factory::Decoder::name   = "Decoder";
const std::string aff3ct::factory::Decoder::prefix = "dec";

void Decoder::build_args(arg_map &req_args, arg_map &opt_args, const std::string p)
{
	req_args[{p+"-info-bits", "K"}] =
		{"positive_int",
		 "useful number of bit transmitted (information bits)."};

	req_args[{p+"-cw-size", "N"}] =
		{"positive_int",
		 "the codeword size."};

	opt_args[{p+"-fra", "F"}] =
		{"positive_int",
		 "set the number of inter frame level to process."};

	opt_args[{p+"-type", "D"}] =
		{"string",
		 "select the algorithm you want to decode the codeword."};

	opt_args[{p+"-implem"}] =
		{"string",
		 "select the implementation of the algorithm to decode."};
}

void Decoder::store_args(const tools::Arguments_reader& ar, parameters &params, const std::string p)
{
	if(ar.exist_arg({p+"-info-bits", "K"})) params.K          = ar.get_arg_int({p+"-info-bits", "K"});
	if(ar.exist_arg({p+"-cw-size",   "N"})) params.N_cw       = ar.get_arg_int({p+"-cw-size",   "N"});
	if(ar.exist_arg({p+"-fra",       "F"})) params.n_frames   = ar.get_arg_int({p+"-fra",       "F"});
	if(ar.exist_arg({p+"-type",      "D"})) params.type       = ar.get_arg    ({p+"-type",      "D"});
	if(ar.exist_arg({p+"-implem"        })) params.implem     = ar.get_arg    ({p+"-implem"        });
	if(ar.exist_arg({p+"-no-sys"        })) params.systematic = false;

	params.R = (float)params.K / (float)params.N_cw;
}

void Decoder::header(params_list& head_dec, const parameters& params)
{
	head_dec.push_back(std::make_pair("Type (D)",params.type));
	if(params.implem.size())
		head_dec.push_back(std::make_pair("Implementation", params.implem));
	head_dec.push_back(std::make_pair("Info. bits (K)", std::to_string(params.K)));
	head_dec.push_back(std::make_pair("Codeword size (N)", std::to_string(params.N_cw)));
	head_dec.push_back(std::make_pair("Code rate (R)", std::to_string(params.R)));
	head_dec.push_back(std::make_pair("Systematic", ((params.systematic) ? "yes" : "no")));
	head_dec.push_back(std::make_pair("Inter frame level", std::to_string(params.n_frames)));
}
