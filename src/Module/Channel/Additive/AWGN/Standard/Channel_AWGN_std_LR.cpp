#include <cmath>

#include "Channel_AWGN_std_LR.hpp"

using namespace aff3ct::module;

template <typename R>
Channel_AWGN_std_LR<R>
::Channel_AWGN_std_LR(const int N, const R& sigma, const int seed, const int n_frames, const std::string name)
: Channel_AWGN_std_LLR<R>(N, sigma, seed, n_frames, name)
{
}

template <typename R>
Channel_AWGN_std_LR<R>
::~Channel_AWGN_std_LR()
{
}

template <typename R>
void Channel_AWGN_std_LR<R>
::add_noise(const R *X_N, R *Y_N)
{
	Channel_AWGN_std_LLR<R>::_add_noise(X_N, Y_N);
	for (auto i = 0; i < this->N * this->n_frames; i++)
		Y_N[i] = std::exp(Y_N[i]);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Channel_AWGN_std_LR<R_32>;
template class aff3ct::module::Channel_AWGN_std_LR<R_64>;
#else
template class aff3ct::module::Channel_AWGN_std_LR<R>;
#endif
// ==================================================================================== explicit template instantiation
