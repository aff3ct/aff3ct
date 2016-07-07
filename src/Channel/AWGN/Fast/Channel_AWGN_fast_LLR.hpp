#ifndef CHANNEL_AWGN_FAST_LLR_HPP_
#define CHANNEL_AWGN_FAST_LLR_HPP_

#include <random>

#include "../../../Tools/PRNG/PRNG_MT19937.hpp"
#include "../../../Tools/PRNG/PRNG_MT19937_fast.hpp"

#include "../../Channel.hpp"

template <typename R>
class Channel_AWGN_fast_LLR : public Channel<R>
{
private:
	const R sigma;
	const R scaling_factor;
	PRNG_MT19937 mt19937; // Mersenne Twister 19937
	PRNG_MT19937_fast mt19937f; // Mersenne Twister 19937

public:
	Channel_AWGN_fast_LLR(const R& sigma, const int seed = 0, const R scaling_factor = 1);
	virtual ~Channel_AWGN_fast_LLR();

	virtual void add_noise(const mipp::vector<R>& X_N, mipp::vector<R>& Y_N);

private:
	inline mipp::Reg<R> get_random_fast();
	inline R            get_random     ();
};

#endif /* CHANNEL_AWGN_FAST_LLR_HPP_ */
