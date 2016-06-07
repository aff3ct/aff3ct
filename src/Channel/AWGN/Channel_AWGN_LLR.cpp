#include "../AWGN/Channel_AWGN_LLR.hpp"

#include <cassert>

template <typename B, typename R>
Channel_AWGN_LLR<B,R>
::Channel_AWGN_LLR(const R& sigma, const int seed, const R scaling_factor)
: Channel_AWGN<B,R>(sigma, seed, scaling_factor)
{
}

template <typename B, typename R>
Channel_AWGN_LLR<B,R>
::~Channel_AWGN_LLR() {
}

template <typename B, typename R>
void Channel_AWGN_LLR<B,R>
::add_noise(const mipp::vector<B>& X_N, mipp::vector<R>& Y_N)
{
	assert(X_N.size() == Y_N.size());
	this->add_gaussian_noise(X_N, Y_N);
}

// ==================================================================================== explicit template instantiation 
#include "../../Tools/types.h"
#ifdef MULTI_PREC
template class Channel_AWGN_LLR<B_8,R_8>;
template class Channel_AWGN_LLR<B_16,R_16>;
template class Channel_AWGN_LLR<B_32,R_32>;
template class Channel_AWGN_LLR<B_64,R_64>;
#else
template class Channel_AWGN_LLR<B,R>;
#endif
// ==================================================================================== explicit template instantiation