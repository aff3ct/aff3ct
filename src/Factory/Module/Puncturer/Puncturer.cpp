#include "Module/Puncturer/NO/Puncturer_NO.hpp"

#include "Puncturer.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Puncturer_name   = "Puncturer";
const std::string aff3ct::factory::Puncturer_prefix = "pct";

Puncturer::parameters
::parameters(const std::string &prefix)
: Factory::parameters(Puncturer_name, Puncturer_name, prefix)
{
}

Puncturer::parameters
::parameters(const std::string &name, const std::string &prefix)
: Factory::parameters(name, Puncturer_name, prefix)
{
}

Puncturer::parameters
::~parameters()
{
}

Puncturer::parameters* Puncturer::parameters
::clone() const
{
	return new Puncturer::parameters(*this);
}

void Puncturer::parameters
::get_description(arg_map &req_args, arg_map &opt_args) const
{
	auto p = this->get_prefix();

	req_args[{p+"-info-bits", "K"}] =
		{"strictly_positive_int",
		 "useful number of bit transmitted (information bits)."};

	req_args[{p+"-fra-size", "N"}] =
		{"strictly_positive_int",
		 "total number of bit transmitted (frame size)."};

	opt_args[{p+"-type"}] =
		{"string",
		 "code puncturer type.",
		 "NO"};

	opt_args[{p+"-fra", "F"}] =
		{"strictly_positive_int",
		 "set the number of inter frame level to process."};
}

void Puncturer::parameters
::store(const arg_val_map &vals)
{
	auto p = this->get_prefix();

	if(exist(vals, {p+"-info-bits", "K"})) this->K        = std::stoi(vals.at({p+"-info-bits", "K"}));
	if(exist(vals, {p+"-fra-size",  "N"})) this->N        = std::stoi(vals.at({p+"-fra-size",  "N"}));
	if(exist(vals, {p+"-fra",       "F"})) this->n_frames = std::stoi(vals.at({p+"-fra",       "F"}));
	if(exist(vals, {p+"-type"          })) this->type     =           vals.at({p+"-type"          });

	this->N_cw = this->N;
}

void Puncturer::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	auto p = this->get_prefix();

	headers[p].push_back(std::make_pair("Type", this->type));
	if (full) headers[p].push_back(std::make_pair("Info. bits (K)", std::to_string(this->K)));
	if (full) headers[p].push_back(std::make_pair("Frame size (N)", std::to_string(this->N)));
	if (full) headers[p].push_back(std::make_pair("Codeword size", std::to_string(this->N_cw)));
	if (full) headers[p].push_back(std::make_pair("Inter frame level", std::to_string(this->n_frames)));
}

template <typename B, typename Q>
module::Puncturer<B,Q>* Puncturer::parameters
::build() const
{
	if (this->type == "NO") return new module::Puncturer_NO<B,Q>(this->K, this->N, this->n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q>
module::Puncturer<B,Q>* Puncturer
::build(const parameters &params)
{
	return params.template build<B,Q>();
}
// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Puncturer<B_8 ,Q_8 >* aff3ct::factory::Puncturer::parameters::build<B_8 ,Q_8 >() const;
template aff3ct::module::Puncturer<B_16,Q_16>* aff3ct::factory::Puncturer::parameters::build<B_16,Q_16>() const;
template aff3ct::module::Puncturer<B_32,Q_32>* aff3ct::factory::Puncturer::parameters::build<B_32,Q_32>() const;
template aff3ct::module::Puncturer<B_64,Q_64>* aff3ct::factory::Puncturer::parameters::build<B_64,Q_64>() const;
template aff3ct::module::Puncturer<B_8 ,Q_8 >* aff3ct::factory::Puncturer::build<B_8 ,Q_8 >(const aff3ct::factory::Puncturer::parameters&);
template aff3ct::module::Puncturer<B_16,Q_16>* aff3ct::factory::Puncturer::build<B_16,Q_16>(const aff3ct::factory::Puncturer::parameters&);
template aff3ct::module::Puncturer<B_32,Q_32>* aff3ct::factory::Puncturer::build<B_32,Q_32>(const aff3ct::factory::Puncturer::parameters&);
template aff3ct::module::Puncturer<B_64,Q_64>* aff3ct::factory::Puncturer::build<B_64,Q_64>(const aff3ct::factory::Puncturer::parameters&);
#else
template aff3ct::module::Puncturer<B,Q>* aff3ct::factory::Puncturer::parameters::build<B,Q>() const;
template aff3ct::module::Puncturer<B,Q>* aff3ct::factory::Puncturer::build<B,Q>(const aff3ct::factory::Puncturer::parameters&);
#endif
// ==================================================================================== explicit template instantiation
