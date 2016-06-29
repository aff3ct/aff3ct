#include "Channel_AWGN.hpp"

#include <algorithm>
#include <cassert>

template <typename R>
Channel_AWGN<R>
::Channel_AWGN(const R& sigma, const int seed, const R scaling_factor)
: sigma(sigma),
  scaling_factor(scaling_factor),
  rd(),
  rd_engine(this->rd()),
  normal_dist(0, sigma)
{
	rd_engine.seed(seed);
}

template <typename R>
Channel_AWGN<R>
::~Channel_AWGN()
{
}

template <typename R>
void Channel_AWGN<R>
::add_gaussian_noise(const mipp::vector<R>& X_N, mipp::vector<R>& Y_N)
{
	assert(X_N.size()     == Y_N.size());
	assert(scaling_factor != 0         );
	assert(sigma          != 0         );

	for (unsigned i = 0; i < X_N.size(); i++)
		Y_N[i] = (X_N[i] + this->normal_dist(this->rd_engine)) * scaling_factor;
}

// ==================================================================================== explicit template instantiation 
#include "../../Tools/types.h"
#ifdef MULTI_PREC
template class Channel_AWGN<R_32>;
template class Channel_AWGN<R_64>;
#else
template class Channel_AWGN<R>;
#endif
// ==================================================================================== explicit template instantiation