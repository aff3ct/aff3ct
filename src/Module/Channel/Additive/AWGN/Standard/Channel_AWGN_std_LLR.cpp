#include <stdexcept>
#include <algorithm>

#include "Channel_AWGN_std_LLR.hpp"

using namespace aff3ct::module;

template <typename R>
Channel_AWGN_std_LLR<R>
::Channel_AWGN_std_LLR(const int N, const R& sigma, const int seed, const int n_frames, const std::string name)
: Channel<R>(N, sigma, n_frames, name.c_str()),
  rd(),
  rd_engine(this->rd()),
  normal_dist(0, this->sigma)
{
	rd_engine.seed(seed);
}

template <typename R>
Channel_AWGN_std_LLR<R>
::~Channel_AWGN_std_LLR()
{
}

template <typename R>
void Channel_AWGN_std_LLR<R>
::set_sigma(const R sigma)
{
	Channel<R>::set_sigma(sigma);
	normal_dist = std::normal_distribution<R>(0, this->sigma);
}

template <typename R>
void Channel_AWGN_std_LLR<R>
::add_noise(const R *X_N, R *Y_N)
{
	for (auto i = 0; i < this->N * this->n_frames; i++)
		Y_N[i] = X_N[i] + this->normal_dist(this->rd_engine);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Channel_AWGN_std_LLR<R_32>;
template class aff3ct::module::Channel_AWGN_std_LLR<R_64>;
#else
template class aff3ct::module::Channel_AWGN_std_LLR<R>;
#endif
// ==================================================================================== explicit template instantiation
