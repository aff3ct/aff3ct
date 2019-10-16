#ifndef SOURCE_RANDOM_FAST_HPP_
#define SOURCE_RANDOM_FAST_HPP_

#include "Tools/Algo/PRNG/PRNG_MT19937.hpp"
#include "Tools/Algo/PRNG/PRNG_MT19937_simd.hpp"
#include "Module/Source/Source.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int>
class Source_random_fast : public Source<B>
{
private:
	tools::PRNG_MT19937      mt19937;      // Mersenne Twister 19937 (scalar)
	tools::PRNG_MT19937_simd mt19937_simd; // Mersenne Twister 19937 (SIMD)

public:
	Source_random_fast(const int K, const int seed = 0, const int n_frames = 1);
	virtual ~Source_random_fast() = default;

protected:
	void _generate(B *U_K, const int frame_id);
};
}
}

#endif /* SOURCE_RANDOM_FAST_HPP_ */
