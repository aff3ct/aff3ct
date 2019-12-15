#ifdef AFF3CT_CHANNEL_GSL

#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

#include "Tools/Exception/exception.hpp"
#include "Tools/Algo/Draw_generator/Gaussian_noise_generator/GSL/Gaussian_noise_generator_GSL.hpp"

using namespace aff3ct::tools;

template <typename R>
Gaussian_noise_generator_GSL<R>
::Gaussian_noise_generator_GSL(const int seed)
: Gaussian_noise_generator<R>(), rng((void*)gsl_rng_alloc(gsl_rng_mt19937))
{
	if (rng == nullptr)
		throw runtime_error(__FILE__, __LINE__, __func__, "'rng' can't be null.");

	this->set_seed(seed);
}

template <typename R>
Gaussian_noise_generator_GSL<R>
::~Gaussian_noise_generator_GSL()
{
	gsl_rng_free((gsl_rng*)rng);
}

template <typename R>
Gaussian_noise_generator_GSL<R>* Gaussian_noise_generator_GSL<R>
::clone() const
{
	Gaussian_noise_generator_GSL<R>* eg = new Gaussian_noise_generator_GSL(*this);
	eg->rng = (void*)gsl_rng_alloc(gsl_rng_mt19937);
	eg->set_seed((int)gsl_rng_get((gsl_rng*)this->rng));
	return eg;
}


template <typename R>
void Gaussian_noise_generator_GSL<R>
::set_seed(const int seed)
{
	gsl_rng_set((gsl_rng*)rng, seed);
}

template <typename R>
void Gaussian_noise_generator_GSL<R>
::generate(R *noise, const unsigned length, const R sigma, const R mu)
{
	for (unsigned i = 0; i < length; i++)
		noise[i] = (R)gsl_ran_gaussian_ziggurat((gsl_rng*)rng, sigma) + mu;
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::tools::Gaussian_noise_generator_GSL<R_32>;
template class aff3ct::tools::Gaussian_noise_generator_GSL<R_64>;
#else
template class aff3ct::tools::Gaussian_noise_generator_GSL<R>;
#endif
// ==================================================================================== explicit template instantiation

#endif
