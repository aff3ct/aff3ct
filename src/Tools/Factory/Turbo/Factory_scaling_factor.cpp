#include "Factory_scaling_factor.hpp"

#include "Tools/params.h"

#include "Tools/Turbo/Scaling_factor/Scaling_factor_NO.hpp"
#include "Tools/Turbo/Scaling_factor/Scaling_factor_seq.hpp"
#include "Tools/Turbo/Scaling_factor/Scaling_factor_vec.hpp"
#include "Tools/Turbo/Scaling_factor/Scaling_factor_array.hpp"
#include "Tools/Turbo/Scaling_factor/Scaling_factor_array_fast.hpp"
#include "Tools/Turbo/Scaling_factor/Scaling_factor_constant.hpp"

template<typename R>
Scaling_factor<R>* Factory_scaling_factor<R>
::build(const t_code_param    &code_params,
        const t_decoder_param &deco_params)
{
	Scaling_factor<R> *scaling_factor = nullptr;

	if (isdigit(deco_params.scaling_factor[0]))
		scaling_factor = new Scaling_factor_constant<R>(code_params.K, deco_params.max_iter, 
		                                                stof(deco_params.scaling_factor));
	else if (deco_params.scaling_factor.find("LTE_VEC") != std::string::npos)
		scaling_factor = new Scaling_factor_vec<R>(code_params.K, deco_params.max_iter);
	else if (deco_params.scaling_factor.find("LTE") != std::string::npos)
		scaling_factor = new Scaling_factor_seq<R>(code_params.K, deco_params.max_iter);
	else if (deco_params.scaling_factor.find("ARRAY") != std::string::npos)
		if(deco_params.implem.find("FAST") != std::string::npos)
			scaling_factor = new Scaling_factor_array_fast<R>(code_params.K, deco_params.max_iter);
		else
			scaling_factor = new Scaling_factor_array<R>(code_params.K, deco_params.max_iter);
	else if (deco_params.scaling_factor.find("NO") != std::string::npos)
		scaling_factor = new Scaling_factor_NO<R>(code_params.K, deco_params.max_iter);

	return scaling_factor;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct Factory_scaling_factor<Q_8>;
template struct Factory_scaling_factor<Q_16>;
template struct Factory_scaling_factor<Q_32>;
template struct Factory_scaling_factor<Q_64>;
#else
template struct Factory_scaling_factor<Q>;
#endif
// ==================================================================================== explicit template instantiation
