#include "Scaling_factor_NO.hpp"

using namespace aff3ct::tools;

template <typename R>
Scaling_factor_NO<R>
::Scaling_factor_NO(const int K, const int n_ite) 
: Scaling_factor<R>(K, n_ite) 
{
}

template <typename R>
void Scaling_factor_NO<R>
::operator()(mipp::vector<R> &ext, const int ite) 
{
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::tools::Scaling_factor_NO<Q_8>;
template class aff3ct::tools::Scaling_factor_NO<Q_16>;
template class aff3ct::tools::Scaling_factor_NO<Q_32>;
template class aff3ct::tools::Scaling_factor_NO<Q_64>;
#else
template class aff3ct::tools::Scaling_factor_NO<Q>;
#endif
// ==================================================================================== explicit template instantiation
