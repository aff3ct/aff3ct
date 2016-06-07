#ifdef CHANNEL_MKL

#ifndef CHANNEL_AWGN_MKL_HPP_
#define CHANNEL_AWGN_MKL_HPP_

#include <mkl_vsl.h>

#include <vector>
#include "../../Tools/MIPP/mipp.h"

#include "../Channel.hpp"

template <typename B, typename R>
class Channel_AWGN_MKL : public Channel<B,R>
{
private:
	const R sigma;
	const R scaling_factor;
	VSLStreamStatePtr stream_state;

public:
	Channel_AWGN_MKL(const R& sigma, const int seed = 0, const R& scaling_factor = 1);
	virtual ~Channel_AWGN_MKL();

	virtual void add_noise(const mipp::vector<B>& X_N, mipp::vector<R>& Y_N) = 0;

protected:
	void add_gaussian_noise(const mipp::vector<B>& X_N, mipp::vector<R>& Y_N);
};

#endif // CHANNEL_AWGN_MKL_HPP_

#endif