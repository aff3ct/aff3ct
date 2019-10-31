#include "Tools/Algo/Draw_generator/Gaussian_noise_generator/Gaussian_noise_generator.hpp"

namespace aff3ct
{
namespace tools
{
template <typename R>
template <class A>
void Gaussian_noise_generator<R>
::generate(std::vector<R,A> &noise, const R sigma, const R mu)
{
	this->generate(noise.data(), (unsigned)noise.size(), sigma, mu);
}
}
}
