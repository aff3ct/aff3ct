#include <algorithm>
#include <cassert>

#include "Channel_AWGN_std_LLR.hpp"

using namespace aff3ct::module;

template <typename R>
Channel_AWGN_std_LLR<R>
::Channel_AWGN_std_LLR(const int N, const R& sigma, const int seed, const int n_frames, const std::string name)
: Channel<R>(N, n_frames, name.c_str()),
  sigma(sigma),
  rd(),
  rd_engine(this->rd()),
  normal_dist(0, sigma)
{
	assert(sigma != 0);

	rd_engine.seed(seed);
}

template <typename R>
Channel_AWGN_std_LLR<R>
::~Channel_AWGN_std_LLR()
{
}

template <typename R>
void Channel_AWGN_std_LLR<R>
::add_noise(const mipp::vector<R>& X_N, mipp::vector<R>& Y_N)
{
	assert(X_N.size() == Y_N.size());

	for (unsigned i = 0; i < X_N.size(); i++)
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
