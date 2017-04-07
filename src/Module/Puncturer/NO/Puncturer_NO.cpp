#include <stdexcept>

#include "Puncturer_NO.hpp"

using namespace aff3ct::module;

template <typename B, typename Q>
Puncturer_NO<B,Q>
::Puncturer_NO(const int K, const int N, const int n_frames, const std::string name)
: Puncturer<B,Q>(K, N, N, n_frames, name)
{
}

template <typename B, typename Q>
Puncturer_NO<B,Q>
::~Puncturer_NO()
{
}

template <typename B, typename Q>
void Puncturer_NO<B,Q>
::puncture(const B *X_N1, B *X_N2) const
{
	std::copy(X_N1, X_N1 + this->N * this->n_frames, X_N2);
}

template <typename B, typename Q>
void Puncturer_NO<B,Q>
::depuncture(const Q *Y_N1, Q *Y_N2) const
{
	std::copy(Y_N1, Y_N1 + this->N * this->n_frames, Y_N2);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Puncturer_NO<B_8,Q_8>;
template class aff3ct::module::Puncturer_NO<B_16,Q_16>;
template class aff3ct::module::Puncturer_NO<B_32,Q_32>;
template class aff3ct::module::Puncturer_NO<B_64,Q_64>;
#else
template class aff3ct::module::Puncturer_NO<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
