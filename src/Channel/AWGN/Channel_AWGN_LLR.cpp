#include "../AWGN/Channel_AWGN_LLR.hpp"

#include <cassert>

template <typename R>
Channel_AWGN_LLR<R>
::Channel_AWGN_LLR(const R& sigma, const int seed, const R scaling_factor)
: Channel_AWGN<R>(sigma, seed, scaling_factor)
{
}

template <typename R>
Channel_AWGN_LLR<R>
::~Channel_AWGN_LLR() {
}

template <typename R>
void Channel_AWGN_LLR<R>
::add_noise(const mipp::vector<R>& X_N, mipp::vector<R>& Y_N)
{
	assert(X_N.size() == Y_N.size());
	this->add_gaussian_noise(X_N, Y_N);
}

// ==================================================================================== explicit template instantiation 
#include "../../Tools/types.h"
#ifdef MULTI_PREC
template class Channel_AWGN_LLR<R_32>;
template class Channel_AWGN_LLR<R_64>;
#else
template class Channel_AWGN_LLR<R>;
#endif
// ==================================================================================== explicit template instantiation