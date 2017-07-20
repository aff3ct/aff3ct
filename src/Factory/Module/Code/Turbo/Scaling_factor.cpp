#include "Tools/Exception/exception.hpp"

#include "Tools/Code/Turbo/Post_processing_SISO/Scaling_factor/Scaling_factor_seq.hpp"
#include "Tools/Code/Turbo/Post_processing_SISO/Scaling_factor/Scaling_factor_vec.hpp"
#include "Tools/Code/Turbo/Post_processing_SISO/Scaling_factor/Scaling_factor_array.hpp"
#include "Tools/Code/Turbo/Post_processing_SISO/Scaling_factor/Scaling_factor_constant.hpp"

#include "Scaling_factor.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

template<typename B, typename Q>
tools::Scaling_factor<B,Q>* Scaling_factor
::build(const parameters& params)
{
	     if (params.type == "CST"    ) return new tools::Scaling_factor_constant<B,Q>(params.n_ite, params.cst        );
	else if (params.type == "LTE_VEC") return new tools::Scaling_factor_vec     <B,Q>(params.n_ite                    );
	else if (params.type == "LTE"    ) return new tools::Scaling_factor_seq     <B,Q>(params.n_ite                    );
	else if (params.type == "ARRAY"  ) return new tools::Scaling_factor_array   <B,Q>(params.n_ite, params.alpha_array);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

void Scaling_factor
::build_args(arg_map &req_args, arg_map &opt_args)
{
	opt_args[{"dec-sf"}] =
		{"string",
		 "scaling factor type.",
		 "CST, LTE, LTE_VEC, ARRAY"};
}

void Scaling_factor
::store_args(const tools::Arguments_reader& ar, parameters &params)
{
	if(ar.exist_arg({"dec-sf"}))
	{
		params.enable = true;
		params.type = ar.get_arg({"dec-sf"});

		if (std::isdigit(params.type[0]))
		{
			params.cst  = std::stof(params.type);
			params.type = "CST";
		}
	}

	if(ar.exist_arg({"dec-ite", "i"})) params.n_ite = ar.get_arg_int({"dec-ite", "i"});
}

void Scaling_factor
::group_args(arg_grp& ar)
{
}

void Scaling_factor
::header(params_list& head_sf, const parameters& params)
{
	if (params.enable)
	{
		head_sf.push_back(std::make_pair("Scaling factor (SF)", params.type));
		head_sf.push_back(std::make_pair("SF iterations", std::to_string(params.n_ite)));
		if (params.type == "CST")
			head_sf.push_back(std::make_pair("SF constant", std::to_string(params.cst)));
	}
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::tools::Scaling_factor<B_8 ,Q_8 >* aff3ct::factory::Scaling_factor::build<B_8 ,Q_8 >(const aff3ct::factory::Scaling_factor::parameters&);
template aff3ct::tools::Scaling_factor<B_16,Q_16>* aff3ct::factory::Scaling_factor::build<B_16,Q_16>(const aff3ct::factory::Scaling_factor::parameters&);
template aff3ct::tools::Scaling_factor<B_32,Q_32>* aff3ct::factory::Scaling_factor::build<B_32,Q_32>(const aff3ct::factory::Scaling_factor::parameters&);
template aff3ct::tools::Scaling_factor<B_64,Q_64>* aff3ct::factory::Scaling_factor::build<B_64,Q_64>(const aff3ct::factory::Scaling_factor::parameters&);
#else
template aff3ct::tools::Scaling_factor<B,Q>* aff3ct::factory::Scaling_factor::build<B,Q>(const aff3ct::factory::Scaling_factor::parameters&);
#endif
// ==================================================================================== explicit template instantiation
