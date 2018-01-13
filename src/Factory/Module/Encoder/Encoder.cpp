#include "Tools/Exception/exception.hpp"

#include "Module/Encoder/NO/Encoder_NO.hpp"
#include "Module/Encoder/AZCW/Encoder_AZCW.hpp"
#include "Module/Encoder/Coset/Encoder_coset.hpp"
#include "Module/Encoder/User/Encoder_user.hpp"

#include "Encoder.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Encoder_name   = "Encoder";
const std::string aff3ct::factory::Encoder_prefix = "enc";

Encoder::parameters
::parameters(const std::string &prefix)
: Factory::parameters(Encoder_name, Encoder_name, prefix)
{
}

Encoder::parameters
::parameters(const std::string &name, const std::string &prefix)
: Factory::parameters(name, Encoder_name, prefix)
{
}

Encoder::parameters
::~parameters()
{
}

Encoder::parameters* Encoder::parameters
::clone() const
{
	return new Encoder::parameters(*this);
}

void Encoder::parameters
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

	opt_args[{p+"-type"}] =
		{"string",
		 "select the type of encoder you want to use.",
		 "NO, AZCW, COSET, USER"};

	opt_args[{p+"-path"}] =
		{"string",
		 "path to a file containing one or a set of pre-computed codewords, to use with \"--enc-type USER\"."};

	opt_args[{p+"-seed", "S"}] =
		{"positive_int",
		 "seed used to initialize the pseudo random generators."};
}

void Encoder::parameters
::store(const arg_val_map &vals)
{
	auto p = this->get_prefix();

	if(exist(vals, {p+"-info-bits", "K"})) this->K          = std::stoi(vals.at({p+"-info-bits", "K"}));
	if(exist(vals, {p+"-cw-size",   "N"})) this->N_cw       = std::stoi(vals.at({p+"-cw-size",   "N"}));
	if(exist(vals, {p+"-fra",       "F"})) this->n_frames   = std::stoi(vals.at({p+"-fra",       "F"}));
	if(exist(vals, {p+"-seed",      "S"})) this->seed       = std::stoi(vals.at({p+"-seed",      "S"}));
	if(exist(vals, {p+"-type"          })) this->type       =           vals.at({p+"-type"          });
	if(exist(vals, {p+"-path"          })) this->path       =           vals.at({p+"-path"          });
	if(exist(vals, {p+"-no-sys"        })) this->systematic = false;

	this->R = (float)this->K / (float)this->N_cw;
}

void Encoder::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	auto p = this->get_prefix();

	headers[p].push_back(std::make_pair("Type", this->type));
	if (full) headers[p].push_back(std::make_pair("Info. bits (K)", std::to_string(this->K)));
	if (full) headers[p].push_back(std::make_pair("Codeword size (N)", std::to_string(this->N_cw)));
	if (full) headers[p].push_back(std::make_pair("Code rate (R)", std::to_string(this->R)));
	if (full) headers[p].push_back(std::make_pair("Inter frame level", std::to_string(this->n_frames)));
	headers[p].push_back(std::make_pair("Systematic", ((this->systematic) ? "yes" : "no")));
	if (this->type == "USER")
		headers[p].push_back(std::make_pair("Path", this->path));
	if (this->type == "COSET" && full)
		headers[p].push_back(std::make_pair("Seed", std::to_string(this->seed)));
}

template <typename B>
module::Encoder<B>* Encoder::parameters
::build() const
{
	     if (this->type == "NO"   ) return new module::Encoder_NO   <B>(this->K,                         this->n_frames);
	else if (this->type == "AZCW" ) return new module::Encoder_AZCW <B>(this->K, this->N_cw,             this->n_frames);
	else if (this->type == "COSET") return new module::Encoder_coset<B>(this->K, this->N_cw, this->seed, this->n_frames);
	else if (this->type == "USER" ) return new module::Encoder_user <B>(this->K, this->N_cw, this->path, this->n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B>
module::Encoder<B>* Encoder
::build(const parameters &params)
{
	return params.template build<B>();
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Encoder<B_8 >* aff3ct::factory::Encoder::parameters::build<B_8 >() const;
template aff3ct::module::Encoder<B_16>* aff3ct::factory::Encoder::parameters::build<B_16>() const;
template aff3ct::module::Encoder<B_32>* aff3ct::factory::Encoder::parameters::build<B_32>() const;
template aff3ct::module::Encoder<B_64>* aff3ct::factory::Encoder::parameters::build<B_64>() const;
template aff3ct::module::Encoder<B_8 >* aff3ct::factory::Encoder::build<B_8 >(const aff3ct::factory::Encoder::parameters&);
template aff3ct::module::Encoder<B_16>* aff3ct::factory::Encoder::build<B_16>(const aff3ct::factory::Encoder::parameters&);
template aff3ct::module::Encoder<B_32>* aff3ct::factory::Encoder::build<B_32>(const aff3ct::factory::Encoder::parameters&);
template aff3ct::module::Encoder<B_64>* aff3ct::factory::Encoder::build<B_64>(const aff3ct::factory::Encoder::parameters&);
#else
template aff3ct::module::Encoder<B>* aff3ct::factory::Encoder::parameters::build<B>() const;
template aff3ct::module::Encoder<B>* aff3ct::factory::Encoder::build<B>(const aff3ct::factory::Encoder::parameters&);
#endif
// ==================================================================================== explicit template instantiation
