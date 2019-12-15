/*!
 * \file
 * \brief Class tools::Gaussian_noise_generator.
 */
#ifndef GAUSSIAN_NOISE_GENERATOR_HPP_
#define GAUSSIAN_NOISE_GENERATOR_HPP_

#include <vector>
#include <memory>

#include "Tools/Algo/Draw_generator/Draw_generator.hpp"

namespace aff3ct
{
namespace tools
{

#if defined(AFF3CT_CHANNEL_GSL) && defined(AFF3CT_CHANNEL_MKL)
enum class Gaussian_noise_generator_implem { STD, FAST, GSL, MKL };
#elif defined(AFF3CT_CHANNEL_GSL)
enum class Gaussian_noise_generator_implem { STD, FAST, GSL };
#elif defined(AFF3CT_CHANNEL_MKL)
enum class Gaussian_noise_generator_implem { STD, FAST, MKL };
#else
enum class Gaussian_noise_generator_implem { STD, FAST };
#endif

template <typename R = float>
class Gaussian_noise_generator : public Draw_generator<R>
{
public:
	Gaussian_noise_generator() = default;
	virtual ~Gaussian_noise_generator() = default;
	virtual Gaussian_noise_generator<R>* clone() const = 0;

	template <class A = std::allocator<R>>
	void generate(std::vector<R,A> &noise, const R sigma, const R mu = 0.0);

	virtual void generate(R *noise, const unsigned length, const R sigma, const R mu = 0.0) = 0;
};

template <typename R = float>
using Gaussian_gen = Gaussian_noise_generator<R>;
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Tools/Algo/Draw_generator/Gaussian_noise_generator/Gaussian_noise_generator.hxx"
#endif

#endif /* GAUSSIAN_NOISE_GENERATOR_HPP_ */
