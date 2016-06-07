#include "Channel_AWGN_LR.hpp"

#include <cassert>
#include <math.h>

template <typename B, typename R>
Channel_AWGN_LR<B,R>
::Channel_AWGN_LR(const R& sigma, const int seed, const R scaling_factor)
: Channel_AWGN<B,R>(sigma, seed, scaling_factor)
{
}

template <typename B, typename R>
Channel_AWGN_LR<B,R>
::~Channel_AWGN_LR()
{
}

template <typename B, typename R>
void Channel_AWGN_LR<B,R>
::add_noise(const mipp::vector<B>& X_N, mipp::vector<R>& Y_N)
{
	assert(X_N.size() == Y_N.size());

	this->add_gaussian_noise(X_N, Y_N);
	for (unsigned i = 0; i < Y_N.size(); i++)
		Y_N[i] = exp(Y_N[i]);
}

// ==================================================================================== explicit template instantiation 
#include "../../Tools/types.h"
#ifdef MULTI_PREC
template class Channel_AWGN_LR<B_8,R_8>;
template class Channel_AWGN_LR<B_16,R_16>;
template class Channel_AWGN_LR<B_32,R_32>;
template class Channel_AWGN_LR<B_64,R_64>;
#else
template class Channel_AWGN_LR<B,R>;
#endif
// ==================================================================================== explicit template instantiation