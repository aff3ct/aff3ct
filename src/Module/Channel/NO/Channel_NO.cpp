#include "Channel_NO.hpp"

using namespace aff3ct::module;

template <typename R>
Channel_NO<R>
::Channel_NO(const int N, const bool add_users, const int n_frames)
: Channel<R>(N, (R)1, n_frames), add_users(add_users)
{
	const std::string name = "Channel_NO";
	this->set_name(name);
}

template <typename R>
Channel_NO<R>
::~Channel_NO() 
{
}

template <typename R>
void Channel_NO<R>
::add_noise(const R *X_N, R *Y_N, const int frame_id)
{
	if (frame_id != -1)
	{
		std::stringstream message;
		message << "'frame_id' has to be equal to -1 ('frame_id' = " << frame_id << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (add_users && this->n_frames > 1)
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
