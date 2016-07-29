#include <cassert>

#include "Encoder_LDPC_fake.hpp"

template <typename B>
Encoder_LDPC_fake<B>
::Encoder_LDPC_fake(const int n_frames)
: Encoder<B>(n_frames)
{
}

template <typename B>
Encoder_LDPC_fake<B>
::~Encoder_LDPC_fake()
{
}

template <typename B>
void Encoder_LDPC_fake<B>
::encode(const mipp::vector<B>& U_K, mipp::vector<B>& X_N)
{
	// TODO: make a fake encoding for the coset approach

	assert(U_K.size() == X_N.size());
	X_N = U_K;
}

// ==================================================================================== explicit template instantiation 
#include "../../Tools/types.h"
#ifdef MULTI_PREC
template class Encoder_LDPC_fake<B_8>;
template class Encoder_LDPC_fake<B_16>;
template class Encoder_LDPC_fake<B_32>;
template class Encoder_LDPC_fake<B_64>;
#else
template class Encoder_LDPC_fake<B>;
#endif
// ==================================================================================== explicit template instantiation