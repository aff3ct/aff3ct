#ifndef GAUSSIAN_NOISE_GENERATOR_HPP_
#define GAUSSIAN_NOISE_GENERATOR_HPP_

#include "../Noise_generator.hpp"


namespace aff3ct
{
namespace tools
{
template <typename R = float>
class Gaussian_noise_generator : public Noise_generator<R>
{
public:
	Gaussian_noise_generator()
	: Noise_generator<R>()
	{
	}

	virtual ~Gaussian_noise_generator()
	{
	}
};

template <typename R = float>
using Gaussian_gen = Gaussian_noise_generator<R>;
}
}

#endif /* GAUSSIAN_NOISE_GENERATOR_HPP_ */
