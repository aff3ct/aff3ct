#include "Tools/Math/utils.h"

#include "Scaling_factor_seq.hpp"

template <typename R>
Scaling_factor_seq<R>
::Scaling_factor_seq(const int K, const int n_ite) 
: Scaling_factor<R>(K, n_ite) 
{
}

template <typename R>
void Scaling_factor_seq<R>
::operator()(mipp::vector<R> &ext, const int ite) 
{
	const auto loop_size = (int)ext.size();

	// if (ite == 1) // sf = 0.50
	// 	for (auto i = 0; i < loop_size; i++)
	// 		ext[i] = div2<R>(ext[i]);
	// else if (ite == this->n_ite) // sf = 1.00
	// {}
	// else // sf = 0.75
		for (auto i = 0; i < loop_size; i++)
			ext[i] = div2<R>(ext[i]) + div4<R>(ext[i]);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Scaling_factor_seq<Q_8>;
template class Scaling_factor_seq<Q_16>;
template class Scaling_factor_seq<Q_32>;
template class Scaling_factor_seq<Q_64>;
#else
template class Scaling_factor_seq<Q>;
#endif
// ==================================================================================== explicit template instantiation
