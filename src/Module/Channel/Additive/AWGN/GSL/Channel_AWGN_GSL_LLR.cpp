#ifdef CHANNEL_GSL

#include <cmath>
#include <stdexcept>
#include <algorithm>

#include "Channel_AWGN_GSL_LLR.hpp"

using namespace aff3ct::module;

template <typename R>
Channel_AWGN_GSL_LLR<R>
::Channel_AWGN_GSL_LLR(const int N, const R& sigma, const int seed, const int n_frames, const std::string name)
: Channel<R>(N, n_frames, name),
  sigma(sigma),
  rng(gsl_rng_alloc(gsl_rng_mt19937))
{
	if (sigma == (R)0)
		throw std::domain_error("aff3ct::module::Channel_AWGN_GSL_LLR: \"sigma\" can't be equal to 0.");
	
	gsl_rng_set(rng, seed);

	if (rng == nullptr)
		throw std::runtime_error("aff3ct::module::Channel_AWGN_GSL_LLR: \"rng\" can't be null.");
}

template <typename R>
Channel_AWGN_GSL_LLR<R>
::~Channel_AWGN_GSL_LLR()
{
	gsl_rng_free(rng);
}

template <typename R>
void Channel_AWGN_GSL_LLR<R>
::_add_noise(const mipp::vector<R>& X_N, mipp::vector<R>& Y_N)
{
	for (unsigned i = 0; i < X_N.size(); i++)
	{
		double r1 = gsl_ran_gaussian(rng, sigma);
		Y_N[i] = (R)X_N[i] + (R)r1;
	}
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Channel_AWGN_GSL_LLR<R_32>;
template class aff3ct::module::Channel_AWGN_GSL_LLR<R_64>;
#else
template class aff3ct::module::Channel_AWGN_GSL_LLR<R>;
#endif
// ==================================================================================== explicit template instantiation

#endif
