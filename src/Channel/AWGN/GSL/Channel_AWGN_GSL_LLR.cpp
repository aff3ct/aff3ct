#ifdef CHANNEL_GSL

#include <cmath>
#include <algorithm>
#include <cassert>

#include "Channel_AWGN_GSL_LLR.hpp"

template <typename R>
Channel_AWGN_GSL_LLR<R>
::Channel_AWGN_GSL_LLR(const R& sigma, const int seed, const R& scaling_factor)
: sigma(sigma),
  scaling_factor((R)scaling_factor),
  rng(gsl_rng_alloc(gsl_rng_mt19937))
{
	gsl_rng_set(rng, seed);
}

template <typename R>
Channel_AWGN_GSL_LLR<R>
::~Channel_AWGN_GSL_LLR()
{
	gsl_rng_free(rng);
}

template <typename R>
void Channel_AWGN_GSL_LLR<R>
::add_noise(const mipp::vector<R>& X_N, mipp::vector<R>& Y_N)
{
	assert(X_N.size() == Y_N.size());
	assert(sigma      != 0         );
	assert(rng        != nullptr   );

	for (unsigned i = 0; i < X_N.size(); i++)
	{
		double r1 = gsl_ran_gaussian(rng, sigma);
		Y_N[i] = ((R)X_N[i] + (R)r1) * scaling_factor;
	}
}

// ==================================================================================== explicit template instantiation 
#include "../../../Tools/types.h"
#ifdef MULTI_PREC
template class Channel_AWGN_GSL_LLR<R_32>;
template class Channel_AWGN_GSL_LLR<R_64>;
#else
template class Channel_AWGN_GSL_LLR<R>;
#endif
// ==================================================================================== explicit template instantiation

#endif