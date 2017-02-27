#ifdef CHANNEL_GSL

#include <cassert>
#include <cmath>

#include "Channel_AWGN_GSL_LR.hpp"

using namespace aff3ct::module;

template <typename R>
Channel_AWGN_GSL_LR<R>
::Channel_AWGN_GSL_LR(const int N, const R& sigma, const int seed, const int n_frames, const std::string name)
: Channel_AWGN_GSL_LLR<R>(N, sigma, seed, n_frames, name)
{
}

template <typename R>
Channel_AWGN_GSL_LR<R>
::~Channel_AWGN_GSL_LR()
{
}

template <typename R>
void Channel_AWGN_GSL_LR<R>
::add_noise(const mipp::vector<R>& X_N, mipp::vector<R>& Y_N)
{
	assert(X_N.size() == Y_N.size());
	
	Channel_AWGN_GSL_LLR<R>::add_noise(X_N, Y_N);
	for (unsigned i = 0; i < X_N.size(); i++)
		Y_N[i] = std::exp(Y_N[i]);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Channel_AWGN_GSL_LR<R_32>;
template class aff3ct::module::Channel_AWGN_GSL_LR<R_64>;
#else
template class aff3ct::module::Channel_AWGN_GSL_LR<R>;
#endif
// ==================================================================================== explicit template instantiation

#endif
