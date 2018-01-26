#include "Decoder.hpp"

using namespace aff3ct::factory;

const std::string aff3ct::factory::Decoder_name   = "Decoder";
const std::string aff3ct::factory::Decoder_prefix = "dec";

Decoder::parameters
::parameters(const std::string &name, const std::string &prefix)
: Factory::parameters(name, Decoder_name, prefix)
{
}

Decoder::parameters
::~parameters()
{
}

Decoder::parameters* Decoder::parameters
::clone() const
{
	return new Decoder::parameters(*this);
}

void Decoder::parameters
::get_description(arg_map &req_args, arg_map &opt_args) const
{
	auto p = this->get_prefix();

	req_args[{p+"-info-bits", "K"}] =
		{"strictly_positive_int",
		 "useful number of bit transmitted (information bits)."};

	req_args[{p+"-cw-size", "N"}] =
		{"strictly_positive_int",
		 "the codeword size."};

	opt_args[{p+"-fra", "F"}] =
		{"strictly_positive_int",
		 "set the number of inter frame level to process."};

	opt_args[{p+"-type", "D"}] =
		{"string",
		 "select the algorithm you want to decode the codeword."};

	opt_args[{p+"-implem"}] =
		{"string",
		 "select the implementation of the algorithm to decode."};
}

void Decoder::parameters
::store(const arg_val_map &vals)
{
	auto p = this->get_prefix();

	if(exist(vals, {p+"-info-bits", "K"})) this->K          = std::stoi(vals.at({p+"-info-bits", "K"}));
	if(exist(vals, {p+"-cw-size",   "N"})) this->N_cw       = std::stoi(vals.at({p+"-cw-size",   "N"}));
	if(exist(vals, {p+"-fra",       "F"})) this->n_frames   = std::stoi(vals.at({p+"-fra",       "F"}));
	if(exist(vals, {p+"-type",      "D"})) this->type       =           vals.at({p+"-type",      "D"});
	if(exist(vals, {p+"-implem"        })) this->implem     =           vals.at({p+"-implem"        });
	if(exist(vals, {p+"-no-sys"        })) this->systematic = false;

	this->R = (float)this->K / (float)this->N_cw;
}

void Decoder::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	auto p = this->get_prefix();

	headers[p].push_back(std::make_pair("Type (D)",this->type));
	if(this->implem.size() && this->type != "ML")
		headers[p].push_back(std::make_pair("Implementation", this->implem));
	if (full) headers[p].push_back(std::make_pair("Info. bits (K)", std::to_string(this->K)));
	if (full) headers[p].push_back(std::make_pair("Codeword size (N)", std::to_string(this->N_cw)));
	if (full) headers[p].push_back(std::make_pair("Code rate (R)", std::to_string(this->R)));
	headers[p].push_back(std::make_pair("Systematic", ((this->systematic) ? "yes" : "no")));
	if (full) headers[p].push_back(std::make_pair("Inter frame level", std::to_string(this->n_frames)));
}
