#ifdef CHANNEL_MKL

#ifndef CHANNEL_AWGN_MKL_LLR_HPP_
#define CHANNEL_AWGN_MKL_LLR_HPP_

#include <mkl_vsl.h>

#include "../../Channel.hpp"

template <typename R>
class Channel_AWGN_MKL_LLR : public Channel<R>
{
private:
	const R sigma;
	VSLStreamStatePtr stream_state;

public:
	Channel_AWGN_MKL_LLR(const int N, const R& sigma, const int seed = 0, const int n_frames = 1, 
	                     const std::string name = "Channel_AWGN_MKL_LLR");
	virtual ~Channel_AWGN_MKL_LLR();

	virtual void add_noise(const mipp::vector<R>& X_N, mipp::vector<R>& Y_N);
};

#endif // CHANNEL_AWGN_MKL_LLR_HPP_

#endif