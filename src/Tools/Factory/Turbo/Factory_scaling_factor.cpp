#include "Tools/Exception/exception.hpp"

#include "Tools/Code/Turbo/Post_processing_SISO/Scaling_factor/Scaling_factor_seq.hpp"
#include "Tools/Code/Turbo/Post_processing_SISO/Scaling_factor/Scaling_factor_vec.hpp"
#include "Tools/Code/Turbo/Post_processing_SISO/Scaling_factor/Scaling_factor_array.hpp"
#include "Tools/Code/Turbo/Post_processing_SISO/Scaling_factor/Scaling_factor_constant.hpp"

#include "Factory_scaling_factor.hpp"

using namespace aff3ct::tools;

template<typename B, typename Q>
Scaling_factor<B,Q>* Factory_scaling_factor<B,Q>
::build(const scaling_factor_parameters& params, const int n_ite)
{
	     if (params.type == "CST"    ) return new Scaling_factor_constant<B,Q>(n_ite, params.cst        );
	else if (params.type == "LTE_VEC") return new Scaling_factor_vec     <B,Q>(n_ite                    );
	else if (params.type == "LTE"    ) return new Scaling_factor_seq     <B,Q>(n_ite                    );
	else if (params.type == "ARRAY"  ) return new Scaling_factor_array   <B,Q>(n_ite, params.alpha_array);

	throw cannot_allocate(__FILE__, __LINE__, __func__);
}

template<typename B, typename Q>
void Factory_scaling_factor<B,Q>
::build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args)
{
	opt_args[{"dec-sf"}] =
		{"string",
		 "scaling factor type.",
		 "CST, LTE, LTE_VEC, ARRAY"};
}

template<typename B, typename Q>
void Factory_scaling_factor<B,Q>
::store_args(const Arguments_reader& ar, scaling_factor_parameters &params)
{
	if(ar.exist_arg({"dec-sf"}))
	{
		params.declaration = ar.get_arg({"dec-sf"});

		params.type = params.declaration;

		if (std::isdigit(params.declaration[0]))
		{
			params.type = "CST";
			params.cst  = std::stof(params.declaration);
		}
	}
}

template<typename B, typename Q>
void Factory_scaling_factor<B,Q>
::group_args(Arguments_reader::arg_grp& ar)
{

}

template<typename B, typename Q>
void Factory_scaling_factor<B,Q>
::header(Header::params_list& head_sf, const scaling_factor_parameters& params)
{
	head_sf.push_back(std::make_pair("Scaling factor", params.declaration));
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct aff3ct::tools::Factory_scaling_factor<B_8,Q_8>;
template struct aff3ct::tools::Factory_scaling_factor<B_16,Q_16>;
template struct aff3ct::tools::Factory_scaling_factor<B_32,Q_32>;
template struct aff3ct::tools::Factory_scaling_factor<B_64,Q_64>;
#else
template struct aff3ct::tools::Factory_scaling_factor<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
