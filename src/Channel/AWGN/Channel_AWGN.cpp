#include "Channel_AWGN.hpp"

#include <algorithm>
#include <cassert>

template <typename B, typename R>
Channel_AWGN<B,R>
::Channel_AWGN(const R& sigma, const int seed, const R scaling_factor)
: sigma(sigma),
  scaling_factor(scaling_factor),
  rd(),
  rd_engine(this->rd()),
  normal_dist(0, sigma)
{
	rd_engine.seed(seed);
}

template <typename B, typename R>
Channel_AWGN<B,R>
::~Channel_AWGN()
{
}

template <typename B, typename R>
void Channel_AWGN<B,R>
::add_gaussian_noise(const mipp::vector<B>& X_N, mipp::vector<R>& Y_N)
{
	assert(X_N.size()     == Y_N.size());
	assert(scaling_factor != 0         );
	assert(sigma          != 0         );

	for (unsigned i = 0; i < X_N.size(); i++)
		Y_N[i] = ((R)X_N[i] + this->normal_dist(this->rd_engine)) * scaling_factor;
}

// ==================================================================================== explicit template instantiation 
#include "../../Tools/types.h"
#ifdef MULTI_PREC
template class Channel_AWGN<B_8,R_8>;
template class Channel_AWGN<B_16,R_16>;
template class Channel_AWGN<B_32,R_32>;
template class Channel_AWGN<B_64,R_64>;
#else
template class Channel_AWGN<B,R>;
#endif
// ==================================================================================== explicit template instantiation