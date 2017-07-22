#include "Module/Coset/Bit/Coset_bit.hpp"
#include "Module/Coset/Real/Coset_real.hpp"

#include "Coset.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Coset::name   = "Coset";
const std::string aff3ct::factory::Coset::prefix = "cst";

template <typename B1, typename B2>
module::Coset<B1,B2>* Coset
::build_bit(const parameters &params)
{
	if (params.type == "STD") return new module::Coset_bit<B1,B2>(params.size, params.n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B, typename R>
module::Coset<B,R>* Coset
::build_real(const parameters &params)
{
	if (params.type == "STD") return new module::Coset_real<B,R>(params.size, params.n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
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
::store_args(const tools::Arguments_reader& ar, parameters &params, const std::string p)
{
	if(ar.exist_arg({p+"-size", "N"})) params.size     = ar.get_arg_int({p+"-size", "N"});
	if(ar.exist_arg({p+"-fra",  "F"})) params.n_frames = ar.get_arg_int({p+"-fra",  "F"});
	if(ar.exist_arg({p+"-type"     })) params.type     = ar.get_arg    ({p+"-type"     });
}

void Coset
::header(params_list& head_pct, const parameters& params)
{
	head_pct.push_back(std::make_pair("Type", params.type));
	head_pct.push_back(std::make_pair("Size (N)", std::to_string(params.size)));
	head_pct.push_back(std::make_pair("Inter frame level", std::to_string(params.n_frames)));
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::module::Coset<B_8 ,B_8 >* aff3ct::factory::Coset::build_bit<B_8 ,B_8 >(const aff3ct::factory::Coset::parameters&);
template aff3ct::module::Coset<B_16,B_16>* aff3ct::factory::Coset::build_bit<B_16,B_16>(const aff3ct::factory::Coset::parameters&);
template aff3ct::module::Coset<B_32,B_32>* aff3ct::factory::Coset::build_bit<B_32,B_32>(const aff3ct::factory::Coset::parameters&);
template aff3ct::module::Coset<B_64,B_64>* aff3ct::factory::Coset::build_bit<B_64,B_64>(const aff3ct::factory::Coset::parameters&);
#else
template aff3ct::module::Coset<B,B>* aff3ct::factory::Coset::build_bit<B,B>(const aff3ct::factory::Coset::parameters&);
#endif

#ifdef MULTI_PREC
template aff3ct::module::Coset<B_8 ,Q_8 >* aff3ct::factory::Coset::build_real<B_8 ,Q_8 >(const aff3ct::factory::Coset::parameters&);
template aff3ct::module::Coset<B_16,Q_16>* aff3ct::factory::Coset::build_real<B_16,Q_16>(const aff3ct::factory::Coset::parameters&);
template aff3ct::module::Coset<B_32,Q_32>* aff3ct::factory::Coset::build_real<B_32,Q_32>(const aff3ct::factory::Coset::parameters&);
template aff3ct::module::Coset<B_64,Q_64>* aff3ct::factory::Coset::build_real<B_64,Q_64>(const aff3ct::factory::Coset::parameters&);
#else
template aff3ct::module::Coset<B,Q>* aff3ct::factory::Coset::build_real<B,Q>(const aff3ct::factory::Coset::parameters&);
#endif
// ==================================================================================== explicit template instantiation
