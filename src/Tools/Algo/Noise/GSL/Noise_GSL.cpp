#ifdef CHANNEL_GSL

#include <stdexcept>

#include "Noise_GSL.hpp"

using namespace aff3ct::tools;

template <typename R>
Noise_GSL<R>
::Noise_GSL(const int seed)
: Noise<R>(),
  rng(gsl_rng_alloc(gsl_rng_mt19937))
{
	if (rng == nullptr)
		throw std::runtime_error("aff3ct::module::Noise_GSL: \"rng\" can't be null.");

	this->set_seed(seed);
}

template <typename R>
void Noise_GSL<R>
::set_seed(const int seed)
{
	gsl_rng_set(rng, seed);
}

template <typename R>
Noise_GSL<R>
::~Noise_GSL()
{
	gsl_rng_free(rng);
}

template <typename R>
void Noise_GSL<R>
::generate(R *noise, const unsigned length, const R sigma)
{
	for (unsigned i = 0; i < length; i++)
		noise[i] = (R)gsl_ran_gaussian(rng, sigma);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::tools::Noise_GSL<R_32>;
template class aff3ct::tools::Noise_GSL<R_64>;
#else
template class aff3ct::tools::Noise_GSL<R>;
#endif
// ==================================================================================== explicit template instantiation

#endif
