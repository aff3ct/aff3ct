#ifndef CHANNEL_AWGN_HPP_
#define CHANNEL_AWGN_HPP_

#include <random>
#include <vector>
#include "../../Tools/MIPP/mipp.h"

#include "../Channel.hpp"

template <typename B, typename R>
class Channel_AWGN : public Channel<B,R>
{
private:
	const R sigma;
	const R scaling_factor;

	std::random_device rd;
	std::linear_congruential_engine<std::uint_fast32_t, 48271, 0, 2147483647> rd_engine; // LCG
	//std::subtract_with_carry_engine<std::uint_fast32_t, 24, 10, 24> rd_engine;
	std::normal_distribution<R> normal_dist;

public:
	Channel_AWGN(const R& sigma, const int seed = 0, const R scaling_factor = 1);
	virtual ~Channel_AWGN();

	virtual void add_noise(const mipp::vector<B>& X_N, mipp::vector<R>& Y_N) = 0;

protected:
	void add_gaussian_noise(const mipp::vector<B>& X_N, mipp::vector<R>& Y_N);
};

#endif /* CHANNEL_AWGN_HPP_ */
