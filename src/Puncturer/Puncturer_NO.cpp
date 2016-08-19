#include "Puncturer_NO.hpp"

template <typename B, typename Q>
Puncturer_NO<B,Q>
::Puncturer_NO(const int K, const int N, const int n_frames, const std::string name)
: Puncturer<B,Q>(K, N, N, n_frames, name.c_str())
{
}

template <typename B, typename Q>
Puncturer_NO<B,Q>
::~Puncturer_NO()
{
}

template <typename B, typename Q>
void Puncturer_NO<B,Q>
::puncture(const mipp::vector<B>& X_N1, mipp::vector<B>& X_N2) const
{
	assert(X_N1.size() == X_N2.size());

	X_N2 = X_N1;
}

template <typename B, typename Q>
void Puncturer_NO<B,Q>
::depuncture(const mipp::vector<Q>& Y_N1, mipp::vector<Q>& Y_N2) const
{
	assert(Y_N1.size() == Y_N2.size());

	Y_N2 = Y_N1;
}

// ==================================================================================== explicit template instantiation 
#include "../Tools/types.h"
#ifdef MULTI_PREC
template class Puncturer_NO<B_8,Q_8>;
template class Puncturer_NO<B_16,Q_16>;
template class Puncturer_NO<B_32,Q_32>;
template class Puncturer_NO<B_64,Q_64>;
#else
template class Puncturer_NO<B,Q>;
#endif
// ==================================================================================== explicit template instantiation