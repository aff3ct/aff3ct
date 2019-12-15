#include "Tools/Algo/Draw_generator/Gaussian_noise_generator/Standard/Gaussian_noise_generator_std.hpp"

using namespace aff3ct::tools;

template <typename R>
Gaussian_noise_generator_std<R>
::Gaussian_noise_generator_std(const int seed)
: Gaussian_noise_generator<R>()
{
	this->set_seed(seed);
}

template <typename R>
Gaussian_noise_generator_std<R>* Gaussian_noise_generator_std<R>
::clone() const
{
	return new Gaussian_noise_generator_std(*this);
}

template <typename R>
void Gaussian_noise_generator_std<R>
::set_seed(const int seed)
{
	rd_engine.seed(seed);
}

template <typename R>
void Gaussian_noise_generator_std<R>
::generate(R *noise, const unsigned length, const R sigma, const R mu)
{
	normal_dist = std::normal_distribution<R>(mu, sigma);

	for (unsigned i = 0; i < length; i++)
		noise[i] = this->normal_dist(this->rd_engine);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::tools::Gaussian_noise_generator_std<R_32>;
template class aff3ct::tools::Gaussian_noise_generator_std<R_64>;
#else
template class aff3ct::tools::Gaussian_noise_generator_std<R>;
#endif
// ==================================================================================== explicit template instantiation
