#include "Tools/Exception/exception.hpp"

#include "Tools/Code/Turbo/Post_processing_SISO/Scaling_factor/Scaling_factor_seq.hpp"
#include "Tools/Code/Turbo/Post_processing_SISO/Scaling_factor/Scaling_factor_vec.hpp"
#include "Tools/Code/Turbo/Post_processing_SISO/Scaling_factor/Scaling_factor_array.hpp"
#include "Tools/Code/Turbo/Post_processing_SISO/Scaling_factor/Scaling_factor_constant.hpp"

#include "Factory_scaling_factor.hpp"

using namespace aff3ct::tools;

template<typename B, typename Q>
Scaling_factor<B,Q>* Factory_scaling_factor
::build(const parameters& params, const int n_ite)
{
	     if (params.type == "CST"    ) return new Scaling_factor_constant<B,Q>(n_ite, params.cst        );
	else if (params.type == "LTE_VEC") return new Scaling_factor_vec     <B,Q>(n_ite                    );
	else if (params.type == "LTE"    ) return new Scaling_factor_seq     <B,Q>(n_ite                    );
	else if (params.type == "ARRAY"  ) return new Scaling_factor_array   <B,Q>(n_ite, params.alpha_array);

	throw cannot_allocate(__FILE__, __LINE__, __func__);
}

void Factory_scaling_factor
::build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args)
{
	opt_args[{"dec-sf"}] =
		{"string",
		 "scaling factor type.",
		 "CST, LTE, LTE_VEC, ARRAY"};
}

void Factory_scaling_factor
::store_args(const Arguments_reader& ar, parameters &params)
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

void Factory_scaling_factor
::group_args(Arguments_reader::arg_grp& ar)
{

}

void Factory_scaling_factor
::header(Header::params_list& head_sf, const parameters& params)
{
	head_sf.push_back(std::make_pair("Scaling factor", params.declaration));
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::tools::Scaling_factor<B_8 ,Q_8 >* aff3ct::tools::Factory_scaling_factor::build<B_8 ,Q_8 >(const aff3ct::tools::Factory_scaling_factor::parameters&, const int);
template aff3ct::tools::Scaling_factor<B_16,Q_16>* aff3ct::tools::Factory_scaling_factor::build<B_16,Q_16>(const aff3ct::tools::Factory_scaling_factor::parameters&, const int);
template aff3ct::tools::Scaling_factor<B_32,Q_32>* aff3ct::tools::Factory_scaling_factor::build<B_32,Q_32>(const aff3ct::tools::Factory_scaling_factor::parameters&, const int);
template aff3ct::tools::Scaling_factor<B_64,Q_64>* aff3ct::tools::Factory_scaling_factor::build<B_64,Q_64>(const aff3ct::tools::Factory_scaling_factor::parameters&, const int);
#else
template aff3ct::tools::Scaling_factor<B,Q>* aff3ct::tools::Factory_scaling_factor::build<B,Q>(const aff3ct::tools::Factory_scaling_factor::parameters&, const int);
#endif
// ==================================================================================== explicit template instantiation
