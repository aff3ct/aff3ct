#ifndef NOISE_FAST_HPP_
#define NOISE_FAST_HPP_

#include <random>

#include "Tools/Algo/PRNG/PRNG_MT19937.hpp"
#include "Tools/Algo/PRNG/PRNG_MT19937_simd.hpp"

#include "../Noise.hpp"

namespace aff3ct
{
namespace tools
{
template <typename R = float>
class Noise_fast : public Noise<R>
{
private:
	tools::PRNG_MT19937      mt19937;      // Mersenne Twister 19937 (scalar)
	tools::PRNG_MT19937_simd mt19937_simd; // Mersenne Twister 19937 (SIMD)

public:
	Noise_fast(const int seed = 0);
	virtual ~Noise_fast();

	virtual void generate(R *noise, const unsigned length, const R sigma);

private:
	inline mipp::Reg<R> get_random_simd();
	inline R            get_random     ();
};
}
}

#endif /* NOISE_FAST_HPP_ */
