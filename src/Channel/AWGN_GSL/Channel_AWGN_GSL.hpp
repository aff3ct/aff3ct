#ifdef CHANNEL_GSL

#ifndef CHANNEL_AWGN_GSL_HPP_
#define CHANNEL_AWGN_GSL_HPP_

#include <gsl/gsl_randist.h>
#include <gsl/gsl_rng.h>

#include <vector>
#include "../../Tools/MIPP/mipp.h"

#include "../Channel.hpp"

template <typename R>
class Channel_AWGN_GSL : public Channel<R>
{
private:
	const R sigma;
	const R scaling_factor;
	gsl_rng *rng;

public:
	Channel_AWGN_GSL(const R& sigma, const int seed = 0, const R& scaling_factor = 1);

	virtual ~Channel_AWGN_GSL();

	virtual void add_noise(const mipp::vector<R>& X_N, mipp::vector<R>& Y_N) = 0;

protected:
	void add_gaussian_noise(const mipp::vector<R>& X_N, mipp::vector<R>& Y_N);
};

#endif // CHANNEL_AWGN_GSL_HPP_

#endif