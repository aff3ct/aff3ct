#ifdef CHANNEL_GSL

#ifndef GAUSSIAN_NOISE_GENERATOR_GSL_HPP_
#define GAUSSIAN_NOISE_GENERATOR_GSL_HPP_

#include <gsl/gsl_randist.h>
#include <gsl/gsl_rng.h>
#include <memory>

#include "../Gaussian_noise_generator.hpp"

namespace aff3ct
{
namespace tools
{
template <typename R = float>
class Gaussian_noise_generator_GSL : public Gaussian_noise_generator<R>
{
private:
	std::unique_ptr<gsl_rng,decltype(&gsl_rng_free)> rng;

public:
	explicit Gaussian_noise_generator_GSL(const int seed = 0);
	virtual ~Gaussian_noise_generator_GSL() = default;

	virtual void set_seed(const int seed);
	virtual void generate(R *noise, const unsigned length, const R sigma, const R mu = 0.0);
};

template <typename R = float>
using Gaussian_gen_GSL = Gaussian_noise_generator_GSL<R>;
}
}

#endif // GAUSSIAN_NOISE_GENERATOR_GSL_HPP_

#endif
