#ifdef CHANNEL_MKL

#ifndef CHANNEL_AWGN_MKL_LR_HPP_
#define CHANNEL_AWGN_MKL_LR_HPP_

#include "Channel_AWGN_MKL_LLR.hpp"

namespace aff3ct
{
namespace module
{
template <typename R = float>
class Channel_AWGN_MKL_LR : public Channel_AWGN_MKL_LLR<R>
{
public:
	Channel_AWGN_MKL_LR(const int N, const R& sigma, const int seed = 0, const int n_frames = 1, 
	                    const std::string name = "Channel_AWGN_MKL_LR");
	virtual ~Channel_AWGN_MKL_LR();

	void add_noise(const mipp::vector<R>& X_N, mipp::vector<R>& Y_N);
};
}
}

#endif // CHANNEL_AWGN_MKL_LR_HPP_

#endif
