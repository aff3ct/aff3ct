#include "Channel_NO.hpp"

using namespace aff3ct::module;

template <typename R>
Channel_NO<R>
::Channel_NO(const int N, const bool add_users, const int n_frames, const std::string name)
: Channel<R>(N, (R)1, n_frames, name), add_users(add_users)
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
	if (add_users)
	{
		std::fill(Y_N, Y_N + this->N, (R)0);
		for (auto f = 0; f < this->n_frames; f++)
			for (auto i = 0; i < this->N; i++)
				Y_N[i] += X_N[f * this->N +i];
	}
	else
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
