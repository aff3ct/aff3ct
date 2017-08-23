#include "Module/Coset/Bit/Coset_bit.hpp"
#include "Module/Coset/Real/Coset_real.hpp"

#include "Coset.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Coset::name   = "Coset";
const std::string aff3ct::factory::Coset::prefix = "cst";

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

void Coset
::build_args(arg_map &req_args, arg_map &opt_args, const std::string p)
{
	req_args[{p+"-size", "N"}] =
		{"positive_int",
		 "coset size."};

	opt_args[{p+"-type"}] =
		{"string",
		 "coset type.",
		 "STD"};

	opt_args[{p+"-fra", "F"}] =
		{"positive_int",
		 "set the number of inter frame level to process."};
}

void Coset
::store_args(const arg_val_map &vals, parameters &params, const std::string p)
{
	if(exist(vals, {p+"-size", "N"})) params.size     = std::stoi(vals.at({p+"-size", "N"}));
	if(exist(vals, {p+"-fra",  "F"})) params.n_frames = std::stoi(vals.at({p+"-fra",  "F"}));
	if(exist(vals, {p+"-type"     })) params.type     =           vals.at({p+"-type"     });
}

void Coset
::make_header(params_list& head_pct, const parameters& params, const bool full)
{
	head_pct.push_back(std::make_pair("Type", params.type));
	if (full) head_pct.push_back(std::make_pair("Size (N)", std::to_string(params.size)));
	if (full) head_pct.push_back(std::make_pair("Inter frame level", std::to_string(params.n_frames)));
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
