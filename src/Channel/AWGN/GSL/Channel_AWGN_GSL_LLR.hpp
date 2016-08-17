#ifdef CHANNEL_GSL

#ifndef CHANNEL_AWGN_GSL_LLR_HPP_
#define CHANNEL_AWGN_GSL_LLR_HPP_

#include <gsl/gsl_randist.h>
#include <gsl/gsl_rng.h>

#include "../../Channel.hpp"

template <typename R>
class Channel_AWGN_GSL_LLR : public Channel<R>
{
private:
	const R sigma;
	gsl_rng *rng;

public:
	Channel_AWGN_GSL_LLR(const R& sigma, const int seed = 0, const std::string name = "Channel_AWGN_GSL_LLR");

	virtual ~Channel_AWGN_GSL_LLR();

	virtual void add_noise(const mipp::vector<R>& X_N, mipp::vector<R>& Y_N);
};

#endif // CHANNEL_AWGN_GSL_LLR_HPP_

#endif