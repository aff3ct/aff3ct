#ifndef CHANNEL_AWGN_FAST_LLR_HPP_
#define CHANNEL_AWGN_FAST_LLR_HPP_

#include <random>

#include "Tools/Algo/PRNG/PRNG_MT19937.hpp"
#include "Tools/Algo/PRNG/PRNG_MT19937_simd.hpp"

#include "../../../Channel.hpp"

namespace aff3ct
{
namespace module
{
template <typename R>
class Channel_AWGN_fast_LLR : public Channel<R>
{
private:
	const R                  sigma;
	tools::PRNG_MT19937      mt19937;      // Mersenne Twister 19937 (scalar)
	tools::PRNG_MT19937_simd mt19937_simd; // Mersenne Twister 19937 (SIMD)

public:
	Channel_AWGN_fast_LLR(const int N, const R& sigma, const int seed = 0, const int n_frames = 1, 
	                      const std::string name = "Channel_AWGN_fast_LLR");
	virtual ~Channel_AWGN_fast_LLR();

	virtual void add_noise(const mipp::vector<R>& X_N, mipp::vector<R>& Y_N);

private:
	inline mipp::Reg<R> get_random_simd();
	inline R            get_random     ();
};
}
}

#endif /* CHANNEL_AWGN_FAST_LLR_HPP_ */
