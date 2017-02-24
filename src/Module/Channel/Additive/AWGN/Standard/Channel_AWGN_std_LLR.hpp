#ifndef CHANNEL_AWGN_STD_LLR_HPP_
#define CHANNEL_AWGN_STD_LLR_HPP_

#include <random>

#include "../../../Channel.hpp"

namespace aff3ct
{
template <typename R>
class Channel_AWGN_std_LLR : public Channel<R>
{
private:
	const R sigma;

	std::random_device          rd;
	// std::minstd_rand            rd_engine; // LCG
	std::mt19937                rd_engine; // Mersenne Twister 19937
	std::normal_distribution<R> normal_dist;

public:
	Channel_AWGN_std_LLR(const int N, const R& sigma, const int seed = 0, const int n_frames = 1, 
	                     const std::string name = "Channel_AWGN_std_LLR");
	virtual ~Channel_AWGN_std_LLR();

	virtual void add_noise(const mipp::vector<R>& X_N, mipp::vector<R>& Y_N);
};
}

#endif /* CHANNEL_AWGN_STD_LLR_HPP_ */
