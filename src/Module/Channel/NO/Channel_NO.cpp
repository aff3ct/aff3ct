#include "Channel_NO.hpp"

using namespace aff3ct::module;

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
::add_noise(const R *X_N, R *Y_N)
{
	std::copy(X_N, X_N + this->N * this->n_frames, Y_N);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Channel_NO<R_32>;
template class aff3ct::module::Channel_NO<R_64>;
#else
template class aff3ct::module::Channel_NO<R>;
#endif
// ==================================================================================== explicit template instantiation
