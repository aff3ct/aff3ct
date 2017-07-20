#include "Coset.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

template <typename B, typename BR>
module::Coset<B,BR>* Coset
::build(const parameters &params)
{
	if (params.type == "STD") return new module::Coset<B,BR>(params.size, params.n_frames);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

void Coset
::build_args(arg_map &req_args, arg_map &opt_args)
{
	req_args[{"cst-size", "N"}] =
		{"positive_int",
		 "coset size."};

	opt_args[{"cst-type"}] =
		{"string",
		 "coset type.",
		 "STD"};

	opt_args[{"cst-fra", "F"}] =
		{"positive_int",
		 "set the number of inter frame level to process."};
}

void Coset
::store_args(const tools::Arguments_reader& ar, parameters &params)
{
	if(ar.exist_arg({"cst-size", "N"})) params.size     = ar.get_arg_int({"cst-size", "N"});
	if(ar.exist_arg({"cst-fra",  "F"})) params.n_frames = ar.get_arg_int({"cst-fra",  "F"});
	if(ar.exist_arg({"cst-type"     })) params.type     = ar.get_arg    ({"cst-type"     });
}

void Coset
::group_args(arg_grp& ar)
{
	ar.push_back({"cst","Coset parameter(s)"});
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
template aff3ct::module::Coset<B_8 ,B_8 >* aff3ct::factory::Coset::build<B_8 ,B_8 >(const aff3ct::factory::Coset::parameters&);
template aff3ct::module::Coset<B_16,B_16>* aff3ct::factory::Coset::build<B_16,B_16>(const aff3ct::factory::Coset::parameters&);
template aff3ct::module::Coset<B_32,B_32>* aff3ct::factory::Coset::build<B_32,B_32>(const aff3ct::factory::Coset::parameters&);
template aff3ct::module::Coset<B_64,B_64>* aff3ct::factory::Coset::build<B_64,B_64>(const aff3ct::factory::Coset::parameters&);
#else
template aff3ct::module::Coset<B,B>* aff3ct::factory::Coset::build<B,B>(const aff3ct::factory::Coset::parameters&);
#endif

#ifdef MULTI_PREC
template aff3ct::module::Coset<B_32,Q_32>* aff3ct::factory::Coset::build<B_32,Q_32>(const aff3ct::factory::Coset::parameters&);
template aff3ct::module::Coset<B_64,Q_64>* aff3ct::factory::Coset::build<B_64,Q_64>(const aff3ct::factory::Coset::parameters&);
#elif defined(PREC_8_BIT) || defined(PREC_16_BIT)
template aff3ct::module::Coset<B,Q>* aff3ct::factory::Coset::build<B,Q>(const aff3ct::factory::Coset::parameters&);
#endif
// ==================================================================================== explicit template instantiation
