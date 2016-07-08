#include <cassert>
#include <algorithm>
#include <cmath>

#include "Quantizer_NO.hpp"

template <typename R, typename Q>
Quantizer_NO<R,Q>
::Quantizer_NO()
{
}

template <typename R, typename Q>
Quantizer_NO<R,Q>
::~Quantizer_NO()
{
}

template<typename R, typename Q>
void Quantizer_NO<R,Q>
::process(mipp::vector<R>& Y_N1, mipp::vector<Q>& Y_N2)
{
	assert(Y_N1.size() == Y_N2.size());

	const auto loop_size = Y_N1.size();
	for (unsigned i = 0; i < loop_size; i++)
		Y_N2[i] = (Q)Y_N1[i];
}

// ==================================================================================== explicit template instantiation 
#include "../Tools/types.h"
#ifdef MULTI_PREC
template class Quantizer_NO<R_8,Q_8>;
template class Quantizer_NO<R_16,Q_16>;
template class Quantizer_NO<R_32,Q_32>;
template class Quantizer_NO<R_64,Q_64>;
#else
template class Quantizer_NO<R,Q>;
#endif
// ==================================================================================== explicit template instantiation