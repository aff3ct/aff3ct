#include <cassert>

#include "Channel_NO.hpp"

template <typename R>
Channel_NO<R>
::Channel_NO(const int N, const int n_frames, const std::string name)
: Channel<R>(N, n_frames, name)
{
}

template <typename R>
Channel_NO<R>
::~Channel_NO() 
{
}

template <typename R>
void Channel_NO<R>
::add_noise(const mipp::vector<R>& X_N, mipp::vector<R>& Y_N)
{
	assert(X_N.size() == Y_N.size());

	Y_N = X_N;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Channel_NO<R_32>;
template class Channel_NO<R_64>;
#else
template class Channel_NO<R>;
#endif
// ==================================================================================== explicit template instantiation
