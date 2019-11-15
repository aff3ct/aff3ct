/*!
 * \file
 * \brief Class tools::Gaussian_noise_generator_fast.
 */
#ifndef GAUSSIAN_NOISE_GENERATOR_FAST_HPP_
#define GAUSSIAN_NOISE_GENERATOR_FAST_HPP_

#include "Tools/Algo/PRNG/PRNG_MT19937.hpp"
#include "Tools/Algo/PRNG/PRNG_MT19937_simd.hpp"
#include "Tools/Algo/Draw_generator/Gaussian_noise_generator/Gaussian_noise_generator.hpp"

namespace aff3ct
{
namespace tools
{
template <typename R = float>
class Gaussian_noise_generator_fast : public Gaussian_noise_generator<R>
{
private:
	tools::PRNG_MT19937      mt19937;      // Mersenne Twister 19937 (scalar)
	tools::PRNG_MT19937_simd mt19937_simd; // Mersenne Twister 19937 (SIMD)

public:
	explicit Gaussian_noise_generator_fast(const int seed = 0);
	virtual ~Gaussian_noise_generator_fast() = default;
	virtual Gaussian_noise_generator_fast<R>* clone() const;

	virtual void set_seed(const int seed);
	virtual void generate(R *noise, const unsigned length, const R sigma, const R mu = 0.0);

private:
	inline mipp::Reg<R> get_random_simd();
	inline R            get_random     ();
};

template <typename R = float>
using Gaussian_gen_fast = Gaussian_noise_generator_fast<R>;
}
}

#endif /* GAUSSIAN_NOISE_GENERATOR_FAST_HPP_ */
