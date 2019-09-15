#ifndef GAUSSIAN_NOISE_GENERATOR_HPP_
#define GAUSSIAN_NOISE_GENERATOR_HPP_

#include <vector>
#include <memory>

#include "Tools/Algo/Draw_generator/Draw_generator.hpp"

namespace aff3ct
{
namespace tools
{
template <typename R = float>
class Gaussian_noise_generator : public Draw_generator<R>
{
public:
	Gaussian_noise_generator() = default;

	virtual ~Gaussian_noise_generator() = default;

	template <class A = std::allocator<R>>
	void generate(std::vector<R,A> &noise, const R sigma, const R mu = 0.0);

	virtual void generate(R *noise, const unsigned length, const R sigma, const R mu = 0.0) = 0;
};

template <typename R = float>
using Gaussian_gen = Gaussian_noise_generator<R>;
}
}

#include "Tools/Algo/Draw_generator/Gaussian_noise_generator/Gaussian_noise_generator.hxx"

#endif /* GAUSSIAN_NOISE_GENERATOR_HPP_ */
