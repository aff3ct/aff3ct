#ifndef CHANNEL_AWGN_FAST_LLR_HPP_
#define CHANNEL_AWGN_FAST_LLR_HPP_

#include <random>

#include "../../Channel.hpp"

template <typename R>
class Channel_AWGN_fast_LLR : public Channel<R>
{
private:
	const R sigma;
	const R scaling_factor;

	std::random_device                rd;
	// std::minstd_rand                  rd_engine; // LCG
	std::mt19937                      rd_engine; // Mersenne Twister 19937
	std::uniform_real_distribution<R> dist;

public:
	Channel_AWGN_fast_LLR(const R& sigma, const int seed = 0, const R scaling_factor = 1);
	virtual ~Channel_AWGN_fast_LLR();

	virtual void add_noise(const mipp::vector<R>& X_N, mipp::vector<R>& Y_N);
};

#endif /* CHANNEL_AWGN_FAST_LLR_HPP_ */
