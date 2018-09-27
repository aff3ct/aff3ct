#ifdef CHANNEL_GSL

#include "Tools/Exception/exception.hpp"

#include "Gaussian_noise_generator_GSL.hpp"

using namespace aff3ct::tools;

template <typename R>
Gaussian_noise_generator_GSL<R>
::Gaussian_noise_generator_GSL(const int seed)
: Gaussian_noise_generator<R>(),
  rng(gsl_rng_alloc(gsl_rng_mt19937), gsl_rng_free)
{
	if (rng == nullptr)
		throw runtime_error(__FILE__, __LINE__, __func__, "'rng' can't be null.");

	this->set_seed(seed);
}

template <typename R>
void Gaussian_noise_generator_GSL<R>
::set_seed(const int seed)
{
	gsl_rng_set(rng.get(), seed);
}

template <typename R>
void Gaussian_noise_generator_GSL<R>
::generate(R *noise, const unsigned length, const R sigma, const R mu)
{
	for (unsigned i = 0; i < length; i++)
		noise[i] = (R)gsl_ran_gaussian(rng.get(), sigma) + mu;
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::tools::Gaussian_noise_generator_GSL<R_32>;
template class aff3ct::tools::Gaussian_noise_generator_GSL<R_64>;
#else
template class aff3ct::tools::Gaussian_noise_generator_GSL<R>;
#endif
// ==================================================================================== explicit template instantiation

#endif
