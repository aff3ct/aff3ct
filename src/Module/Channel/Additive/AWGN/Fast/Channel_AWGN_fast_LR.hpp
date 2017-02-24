#ifndef CHANNEL_AWGN_FAST_LR_HPP_
#define CHANNEL_AWGN_FAST_LR_HPP_

#include "Channel_AWGN_fast_LLR.hpp"

namespace aff3ct
{
template <typename R>
class Channel_AWGN_fast_LR : public Channel_AWGN_fast_LLR<R>
{
public:
	Channel_AWGN_fast_LR(const int N, const R& sigma, const int seed = 0, const int n_frames = 1, 
	                     const std::string name = "Channel_AWGN_fast_LR");
	virtual ~Channel_AWGN_fast_LR();

	void add_noise(const mipp::vector<R>& X_N, mipp::vector<R>& Y_N);
};
}

#endif // CHANNEL_AWGN_FAST_LR_HPP_
