#ifdef CHANNEL_MKL

#ifndef CHANNEL_AWGN_MKL_LR_HPP_
#define CHANNEL_AWGN_MKL_LR_HPP_

#include <vector>
#include "../../Tools/MIPP/mipp.h"

#include "Channel_AWGN_MKL.hpp"

template <typename R>
class Channel_AWGN_MKL_LR : public Channel_AWGN_MKL<R>
{
public:
	Channel_AWGN_MKL_LR(const R& sigma, const int seed = 0, const R& scaling_factor = 1);
	virtual ~Channel_AWGN_MKL_LR();

	void add_noise(const mipp::vector<R>& X_N, mipp::vector<R>& Y_N);
};

#endif // CHANNEL_AWGN_MKL_LR_HPP_

#endif