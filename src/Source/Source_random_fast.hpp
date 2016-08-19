#ifndef SOURCE_RANDOM_FAST_HPP_
#define SOURCE_RANDOM_FAST_HPP_

#include <vector>
#include "../Tools/MIPP/mipp.h"

#include "../Tools/PRNG/PRNG_MT19937.hpp"
#include "../Tools/PRNG/PRNG_MT19937_simd.hpp"

#include "Source.hpp"

template <typename B>
class Source_random_fast : public Source<B>
{
private:
	PRNG_MT19937      mt19937;      // Mersenne Twister 19937 (scalar)
	PRNG_MT19937_simd mt19937_simd; // Mersenne Twister 19937 (SIMD)

public:
	Source_random_fast(const int K, const int seed = 0, const int n_frames = 1, const std::string name = "Source_random_fast");
	virtual ~Source_random_fast();

	void generate(mipp::vector<B>& U_K);
};

#endif /* SOURCE_RANDOM_FAST_HPP_ */
