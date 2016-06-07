#ifdef CHANNEL_MKL

#ifndef CHANNEL_AWGN_MKL_LLR_HPP_
#define CHANNEL_AWGN_MKL_LLR_HPP_

#include <vector>
#include "../../Tools/MIPP/mipp.h"

#include "Channel_AWGN_MKL.hpp"

template <typename B, typename R>
class Channel_AWGN_MKL_LLR : public Channel_AWGN_MKL<B,R>
{
public:
	Channel_AWGN_MKL_LLR(const R& sigma, const int seed = 0, const R& scaling_factor = 1);
	virtual ~Channel_AWGN_MKL_LLR();

	void add_noise(const mipp::vector<B>& X_N, mipp::vector<R>& Y_N);
};

#endif // CHANNEL_AWGN_MKL_LLR_HPP_

#endif
