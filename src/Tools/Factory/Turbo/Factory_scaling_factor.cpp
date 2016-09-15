#include "Factory_scaling_factor.hpp"

#include "Tools/params.h"

#include "Tools/Code/Turbo/Scaling_factor/Scaling_factor_NO.hpp"
#include "Tools/Code/Turbo/Scaling_factor/Scaling_factor_seq.hpp"
#include "Tools/Code/Turbo/Scaling_factor/Scaling_factor_vec.hpp"
#include "Tools/Code/Turbo/Scaling_factor/Scaling_factor_array.hpp"
#include "Tools/Code/Turbo/Scaling_factor/Scaling_factor_array_fast.hpp"
#include "Tools/Code/Turbo/Scaling_factor/Scaling_factor_constant.hpp"

template<typename R>
Scaling_factor<R>* Factory_scaling_factor<R>
::build(const parameters &params)
{
	Scaling_factor<R> *scaling_factor = nullptr;

	if (isdigit(params.decoder.scaling_factor[0]))
		scaling_factor = new Scaling_factor_constant<R>(params.code.K, params.decoder.max_iter,
		                                                stof(params.decoder.scaling_factor));
	else if (params.decoder.scaling_factor.find("LTE_VEC") != std::string::npos)
		scaling_factor = new Scaling_factor_vec<R>(params.code.K, params.decoder.max_iter);
	else if (params.decoder.scaling_factor.find("LTE") != std::string::npos)
		scaling_factor = new Scaling_factor_seq<R>(params.code.K, params.decoder.max_iter);
	else if (params.decoder.scaling_factor.find("ARRAY") != std::string::npos)
		if(params.decoder.implem.find("FAST") != std::string::npos)
			scaling_factor = new Scaling_factor_array_fast<R>(params.code.K, params.decoder.max_iter);
		else
			scaling_factor = new Scaling_factor_array<R>(params.code.K, params.decoder.max_iter);
	else if (params.decoder.scaling_factor.find("NO") != std::string::npos)
		scaling_factor = new Scaling_factor_NO<R>(params.code.K, params.decoder.max_iter);

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
