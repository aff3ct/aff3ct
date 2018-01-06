#include "Module/Coset/Bit/Coset_bit.hpp"
#include "Module/Coset/Real/Coset_real.hpp"

#include "Coset.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Coset::name   = "Coset";
const std::string aff3ct::factory::Coset::prefix = "cst";

Coset::parameters
::parameters(const std::string prefix)
: Factory::parameters(Coset::name, Coset::name, prefix)
{
}

Coset::parameters
::~parameters()
{
}

Coset::parameters* Coset::parameters
::clone() const
{
	return new Coset::parameters(*this);
}

void Coset::parameters
::get_description(tools::Argument_map_info &req_args, tools::Argument_map_info &opt_args) const
{
	auto p = this->get_prefix();

	req_args.add(
		{p+"-size", "N"},
		tools::Integer(tools::Positive(), tools::Non_zero()),
		"coset size.");

	opt_args.add(
		{p+"-type"},
		tools::Text(tools::Including_set("STD")),
		"coset type.");

	opt_args.add(
		{p+"-fra", "F"},
		tools::Integer(tools::Positive(), tools::Non_zero()),
		"set the number of inter frame level to process.");
}

void Coset::parameters
::store(const tools::Argument_map_value &vals)
{
	auto p = this->get_prefix();

	if(vals.exist({p+"-size", "N"})) this->size     = vals.to_int({p+"-size", "N"});
	if(vals.exist({p+"-fra",  "F"})) this->n_frames = vals.to_int({p+"-fra",  "F"});
	if(vals.exist({p+"-type"     })) this->type     = vals.at    ({p+"-type"     });
}

void Coset::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	auto p = this->get_prefix();

	headers[p].push_back(std::make_pair("Type", this->type));
	if (full) headers[p].push_back(std::make_pair("Size (N)", std::to_string(this->size)));
	if (full) headers[p].push_back(std::make_pair("Inter frame level", std::to_string(this->n_frames)));
}

template <typename B1, typename B2>
module::Coset<B1,B2>* Coset::parameters
::build_bit() const
{
	if (type == "STD") return new module::Coset_bit<B1,B2>(size, n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename R>
module::Coset<B,R>* Coset::parameters
::build_real() const
{
	if (type == "STD") return new module::Coset_real<B,R>(size, n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B1, typename B2>
module::Coset<B1,B2>* Coset
::build_bit(const parameters &params)
{
	return params.template build_bit<B1,B2>();
}

template <typename B, typename R>
module::Coset<B,R>* Coset
::build_real(const parameters &params)
{
	return params.template build_real<B,R>();
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Coset<B_8 ,B_8 >* aff3ct::factory::Coset::parameters::build_bit<B_8 ,B_8 >() const;
template aff3ct::module::Coset<B_16,B_16>* aff3ct::factory::Coset::parameters::build_bit<B_16,B_16>() const;
template aff3ct::module::Coset<B_32,B_32>* aff3ct::factory::Coset::parameters::build_bit<B_32,B_32>() const;
template aff3ct::module::Coset<B_64,B_64>* aff3ct::factory::Coset::parameters::build_bit<B_64,B_64>() const;
template aff3ct::module::Coset<B_8 ,B_8 >* aff3ct::factory::Coset::build_bit<B_8 ,B_8 >(const aff3ct::factory::Coset::parameters&);
template aff3ct::module::Coset<B_16,B_16>* aff3ct::factory::Coset::build_bit<B_16,B_16>(const aff3ct::factory::Coset::parameters&);
template aff3ct::module::Coset<B_32,B_32>* aff3ct::factory::Coset::build_bit<B_32,B_32>(const aff3ct::factory::Coset::parameters&);
template aff3ct::module::Coset<B_64,B_64>* aff3ct::factory::Coset::build_bit<B_64,B_64>(const aff3ct::factory::Coset::parameters&);
#else
template aff3ct::module::Coset<B,B>* aff3ct::factory::Coset::parameters::build_bit<B,B>() const;
template aff3ct::module::Coset<B,B>* aff3ct::factory::Coset::build_bit<B,B>(const aff3ct::factory::Coset::parameters&);
#endif

#ifdef MULTI_PREC
template aff3ct::module::Coset<B_8 ,Q_8 >* aff3ct::factory::Coset::parameters::build_real<B_8 ,Q_8 >() const;
template aff3ct::module::Coset<B_16,Q_16>* aff3ct::factory::Coset::parameters::build_real<B_16,Q_16>() const;
template aff3ct::module::Coset<B_32,Q_32>* aff3ct::factory::Coset::parameters::build_real<B_32,Q_32>() const;
template aff3ct::module::Coset<B_64,Q_64>* aff3ct::factory::Coset::parameters::build_real<B_64,Q_64>() const;
template aff3ct::module::Coset<B_8 ,Q_8 >* aff3ct::factory::Coset::build_real<B_8 ,Q_8 >(const aff3ct::factory::Coset::parameters&);
template aff3ct::module::Coset<B_16,Q_16>* aff3ct::factory::Coset::build_real<B_16,Q_16>(const aff3ct::factory::Coset::parameters&);
template aff3ct::module::Coset<B_32,Q_32>* aff3ct::factory::Coset::build_real<B_32,Q_32>(const aff3ct::factory::Coset::parameters&);
template aff3ct::module::Coset<B_64,Q_64>* aff3ct::factory::Coset::build_real<B_64,Q_64>(const aff3ct::factory::Coset::parameters&);
#else
template aff3ct::module::Coset<B,Q>* aff3ct::factory::Coset::parameters::build_real<B,Q>() const;
template aff3ct::module::Coset<B,Q>* aff3ct::factory::Coset::build_real<B,Q>(const aff3ct::factory::Coset::parameters&);
#endif
// ==================================================================================== explicit template instantiation
