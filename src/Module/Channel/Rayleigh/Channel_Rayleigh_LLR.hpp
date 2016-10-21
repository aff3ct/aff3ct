#ifndef CHANNEL_RAYLEIGH_LLR_HPP_
#define CHANNEL_RAYLEIGH_LLR_HPP_

#include <random>

#include "../Channel.hpp"

template <typename R>
class Channel_Rayleigh_LLR : public Channel<R>
{
private:
	const R sigma;

	std::random_device          rd;
	// std::minstd_rand            rd_engine; // LCG
	std::mt19937                rd_engine; // Mersenne Twister 19937
	std::normal_distribution<R> normal_dist_n;
	std::normal_distribution<R> normal_dist_h;

public:
	Channel_Rayleigh_LLR(const int N, const R& sigma, const int seed = 0, const int n_frames = 1, 
	                     const std::string name = "Channel_Rayleigh_LLR");
	virtual ~Channel_Rayleigh_LLR();

	virtual void add_noise(const mipp::vector<R>& X_N, mipp::vector<R>& Y_N);
};

#endif /* CHANNEL_RAYLEIGH_LLR_HPP_ */
