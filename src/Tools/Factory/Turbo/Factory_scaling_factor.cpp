#include "Tools/Exception/exception.hpp"

#include "Tools/Code/Turbo/Post_processing_SISO/Scaling_factor/Scaling_factor_seq.hpp"
#include "Tools/Code/Turbo/Post_processing_SISO/Scaling_factor/Scaling_factor_vec.hpp"
#include "Tools/Code/Turbo/Post_processing_SISO/Scaling_factor/Scaling_factor_array.hpp"
#include "Tools/Code/Turbo/Post_processing_SISO/Scaling_factor/Scaling_factor_constant.hpp"

#include "Factory_scaling_factor.hpp"

using namespace aff3ct::tools;

template<typename B, typename R>
Scaling_factor<B,R>* Factory_scaling_factor<B,R>
::build(const std::string         type,
        const int                 n_ite,
        const float               cst,
        const mipp::vector<float> alpha_array)
{
	     if (type == "CST"    ) return new Scaling_factor_constant<B,R>(n_ite, cst        );
	else if (type == "LTE_VEC") return new Scaling_factor_vec     <B,R>(n_ite             );
	else if (type == "LTE"    ) return new Scaling_factor_seq     <B,R>(n_ite             );
	else if (type == "ARRAY"  ) return new Scaling_factor_array   <B,R>(n_ite, alpha_array);

	throw cannot_allocate(__FILE__, __LINE__, __func__);
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
