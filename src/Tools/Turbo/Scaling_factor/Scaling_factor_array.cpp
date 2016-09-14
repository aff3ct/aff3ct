#include <cassert>

#include "../../../Tools/Math/utils.h"

#include "Scaling_factor_array.hpp"

template <typename R>
Scaling_factor_array<R>
::Scaling_factor_array(const int K, const int n_ite) 
: Scaling_factor<R>(K, n_ite) 
{
}

template <typename R>
void Scaling_factor_array<R>
::operator()(mipp::vector<R> &ext, const int ite) 
{
	assert((unsigned)ite < alpha_array.size());
	
	const auto loop_size = (int)ext.size();

	for (auto i = 0; i < loop_size; i++)
		ext[i] *= alpha_array[ite];
}

// ==================================================================================== explicit template instantiation 
#include "../../types.h"
#ifdef MULTI_PREC
template class Scaling_factor_array<Q_8>;
template class Scaling_factor_array<Q_16>;
template class Scaling_factor_array<Q_32>;
template class Scaling_factor_array<Q_64>;
#else
template class Scaling_factor_array<Q>;
#endif
// ==================================================================================== explicit template instantiation
