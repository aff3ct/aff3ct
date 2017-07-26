#include "Tools/Exception/exception.hpp"

#include "Tools/Code/Turbo/Post_processing_SISO/Scaling_factor/Scaling_factor_seq.hpp"
#include "Tools/Code/Turbo/Post_processing_SISO/Scaling_factor/Scaling_factor_vec.hpp"
#include "Tools/Code/Turbo/Post_processing_SISO/Scaling_factor/Scaling_factor_array.hpp"
#include "Tools/Code/Turbo/Post_processing_SISO/Scaling_factor/Scaling_factor_constant.hpp"

#include "Scaling_factor.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Scaling_factor::name   = "Scaling factor";
const std::string aff3ct::factory::Scaling_factor::prefix = "sf";

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
::build_args(arg_map &req_args, arg_map &opt_args, const std::string p)
{
	opt_args[{p+"-type"}] =
		{"string",
		 "scaling factor type.",
		 "CST, LTE, LTE_VEC, ARRAY"};

	opt_args[{p+"-ite"}] =
		{"positive_int",
		 "number of iterations."};
}

void Scaling_factor
::store_args(const arg_val_map &vals, parameters &params, const std::string p)
{
	if(exist(vals, {p+"-type"}))
	{
		params.enable = true;
		params.type = vals.at({p+"-type"});

		if (std::isdigit(params.type[0]))
		{
			params.cst  = std::stof(params.type);
			params.type = "CST";
		}
	}

	if(exist(vals, {p+"-ite", "i"})) params.n_ite = std::stoi(vals.at({p+"-ite", "i"}));
}

void Scaling_factor
::make_header(params_list& head_sf, const parameters& params, const bool full)
{
	if (params.enable)
	{
		head_sf.push_back(std::make_pair("Scaling factor (SF)", params.type));
		head_sf.push_back(std::make_pair("SF iterations", std::to_string(params.n_ite)));
		if (params.type == "CST")
			head_sf.push_back(std::make_pair("SF constant", std::to_string(params.cst)));
	}
	else
	{
		head_sf.push_back(std::make_pair("Scaling factor (SF)", "off"));
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
