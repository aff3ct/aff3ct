#include <stdexcept>

#include "Tools/Code/Turbo/Scaling_factor/Scaling_factor_NO.hpp"
#include "Tools/Code/Turbo/Scaling_factor/Scaling_factor_seq.hpp"
#include "Tools/Code/Turbo/Scaling_factor/Scaling_factor_vec.hpp"
#include "Tools/Code/Turbo/Scaling_factor/Scaling_factor_array.hpp"
#include "Tools/Code/Turbo/Scaling_factor/Scaling_factor_array_fast.hpp"
#include "Tools/Code/Turbo/Scaling_factor/Scaling_factor_constant.hpp"

#include "Factory_scaling_factor.hpp"

using namespace aff3ct::tools;

template<typename R>
Scaling_factor<R>* Factory_scaling_factor<R>
::build(const std::string type,
        const int         K,
        const int         n_ite,
        const float       cst)
{
	     if (type == "CST"       ) return new Scaling_factor_constant  <R>(K, n_ite, cst);
	else if (type == "LTE_VEC"   ) return new Scaling_factor_vec       <R>(K, n_ite     );
	else if (type == "LTE"       ) return new Scaling_factor_seq       <R>(K, n_ite     );
	else if (type == "ARRAY"     ) return new Scaling_factor_array     <R>(K, n_ite     );
	else if (type == "ARRAY_FAST") return new Scaling_factor_array_fast<R>(K, n_ite     );
	else if (type == "NO"        ) return new Scaling_factor_NO        <R>(K, n_ite     );

	throw std::runtime_error("aff3ct::tools::Factory_scaling_factor: the factory could not allocate the object.");
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct aff3ct::tools::Factory_scaling_factor<Q_8>;
template struct aff3ct::tools::Factory_scaling_factor<Q_16>;
template struct aff3ct::tools::Factory_scaling_factor<Q_32>;
template struct aff3ct::tools::Factory_scaling_factor<Q_64>;
#else
template struct aff3ct::tools::Factory_scaling_factor<Q>;
#endif
// ==================================================================================== explicit template instantiation
