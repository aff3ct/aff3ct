#include "Decoder.hpp"

using namespace aff3ct::factory;

const std::string aff3ct::factory::Decoder::name   = "Decoder";
const std::string aff3ct::factory::Decoder::prefix = "dec";

Decoder::parameters
::parameters(const std::string name, const std::string prefix)
: Factory::parameters(name, Decoder::name, prefix)
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
::get_description(tools::Argument_map_info &req_args, tools::Argument_map_info &opt_args) const
{
	auto p = this->get_prefix();

	req_args.add(
		{p+"-cw-size", "N"},
		tools::Integer(tools::Positive(), tools::Non_zero()),
		"the codeword size.");

	req_args.add(
		{p+"-info-bits", "K"},
		tools::Integer(tools::Positive(), tools::Non_zero()),
		"useful number of bit transmitted (information bits).");

	opt_args.add(
		{p+"-fra", "F"},
		tools::Integer(tools::Positive(), tools::Non_zero()),
		"set the number of inter frame level to process.");

	opt_args.add(
		{p+"-type", "D"},
		tools::Text(tools::Including_set<std::string>()),
		"select the algorithm you want to decode the codeword.");

	opt_args.add(
		{p+"-implem"},
		tools::Text(tools::Including_set<std::string>()),
		"select the implementation of the algorithm to decode.");
}

void Decoder::parameters
::store(const tools::Argument_map_value &vals)
{
	auto p = this->get_prefix();

	if(vals.exist({p+"-info-bits", "K"})) this->K          = vals.to_int({p+"-info-bits", "K"});
	if(vals.exist({p+"-cw-size",   "N"})) this->N_cw       = vals.to_int({p+"-cw-size",   "N"});
	if(vals.exist({p+"-fra",       "F"})) this->n_frames   = vals.to_int({p+"-fra",       "F"});
	if(vals.exist({p+"-type",      "D"})) this->type       = vals.at    ({p+"-type",      "D"});
	if(vals.exist({p+"-implem"        })) this->implem     = vals.at    ({p+"-implem"        });
	if(vals.exist({p+"-no-sys"        })) this->systematic = false;

	this->R = (float)this->K / (float)this->N_cw;
}

void Decoder::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	auto p = this->get_prefix();

	headers[p].push_back(std::make_pair("Type (D)",this->type));
	if(this->implem.size())
		headers[p].push_back(std::make_pair("Implementation", this->implem));
	if (full) headers[p].push_back(std::make_pair("Info. bits (K)", std::to_string(this->K)));
	if (full) headers[p].push_back(std::make_pair("Codeword size (N)", std::to_string(this->N_cw)));
	if (full) headers[p].push_back(std::make_pair("Code rate (R)", std::to_string(this->R)));
	headers[p].push_back(std::make_pair("Systematic", ((this->systematic) ? "yes" : "no")));
	if (full) headers[p].push_back(std::make_pair("Inter frame level", std::to_string(this->n_frames)));
}
