#include <algorithm>
#include <sstream>
#include <string>

#include "Tools/Exception/exception.hpp"
#include "Module/Channel/NO/Channel_NO.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename R>
Channel_NO<R>
::Channel_NO(const int N, const bool add_users)
: Channel<R>(N), add_users(add_users)
{
	const std::string name = "Channel_NO";
	this->set_name(name);

	if (add_users)
		this->set_single_wave(true);
}

template <typename R>
Channel_NO<R>* Channel_NO<R>
::clone() const
{
	auto m = new Channel_NO(*this);
	m->deep_copy(*this);
	return m;
}

template <typename R>
void Channel_NO<R>
::_add_noise(const float *CP, const R *X_N, R *Y_N, const size_t frame_id)
{
	if (add_users && this->n_frames > 1) // n_frames_per_wave = n_frames
	{
		std::fill(Y_N, Y_N + this->N, (R)0);
		for (size_t f = 0; f < this->n_frames; f++)
			for (auto i = 0; i < this->N; i++)
				Y_N[i] += X_N[f * this->N +i];
	}
	else // n_frames_per_wave = 1
	{
		std::copy(X_N + 0 * this->N,
		          X_N + 1 * this->N,
		          Y_N + 0 * this->N);
	}
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Channel_NO<R_32>;
template class aff3ct::module::Channel_NO<R_64>;
#else
template class aff3ct::module::Channel_NO<R>;
#endif
// ==================================================================================== explicit template instantiation
