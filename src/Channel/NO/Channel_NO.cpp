#include <cassert>

#include "Channel_NO.hpp"

template <typename B, typename R>
Channel_NO<B,R>
::Channel_NO()
{
}

template <typename B, typename R>
Channel_NO<B,R>
::~Channel_NO() 
{
}

template <typename B, typename R>
void Channel_NO<B,R>
::add_noise(const mipp::vector<B>& X_N, mipp::vector<R>& Y_N)
{
	assert(X_N.size() == Y_N.size());

	auto size = X_N.size();
	for (unsigned i = 0; i < size; i++)
		Y_N[i] = (R)X_N[i];
}

// ==================================================================================== explicit template instantiation 
#include "../../Tools/types.h"
#ifdef MULTI_PREC
template class Channel_NO<B_8,R_8>;
template class Channel_NO<B_16,R_16>;
template class Channel_NO<B_32,R_32>;
template class Channel_NO<B_64,R_64>;
#else
template class Channel_NO<B,R>;
#endif
// ==================================================================================== explicit template instantiation