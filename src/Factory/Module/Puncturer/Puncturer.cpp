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
::get_description(tools::Argument_map_info &args) const
{
	auto p = this->get_prefix();

	args.add(
		{p+"-info-bits", "K"},
		tools::Integer(tools::Positive(), tools::Non_zero()),
		"useful number of bit transmitted (information bits).",
		tools::arg_rank::REQUIRED);

	args.add(
		{p+"-fra-size", "N"},
		tools::Integer(tools::Positive(), tools::Non_zero()),
		"useful number of bit transmitted (information bits).",
		tools::arg_rank::REQUIRED);

	args.add(
		{p+"-fra", "F"},
		tools::Integer(tools::Positive(), tools::Non_zero()),
		"set the number of inter frame level to process.");

	args.add(
		{p+"-type"},
		tools::Text(tools::Including_set("NO")),
		"code puncturer type.");
}

void Puncturer::parameters
::store(const tools::Argument_map_value &vals)
{
	auto p = this->get_prefix();

	if(vals.exist({p+"-info-bits", "K"})) this->K        = vals.to_int({p+"-info-bits", "K"});
	if(vals.exist({p+"-fra-size",  "N"})) this->N        = vals.to_int({p+"-fra-size",  "N"});
	if(vals.exist({p+"-fra",       "F"})) this->n_frames = vals.to_int({p+"-fra",       "F"});
	if(vals.exist({p+"-type"          })) this->type     = vals.at    ({p+"-type"          });

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
