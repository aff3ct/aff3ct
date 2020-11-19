#include <algorithm>
#include <sstream>
#include <string>

#include "Tools/Exception/exception.hpp"
#include "Module/Channel/User/Channel_user_add.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename R>
Channel_user_add<R>
::Channel_user_add(const int N, const std::string &filename, const bool add_users)
: Channel_user<R>(N, filename, add_users)
{
	const std::string name = "Channel_user_add";
	this->set_name(name);
}

template <typename R>
Channel_user_add<R>* Channel_user_add<R>
::clone() const
{
	auto m = new Channel_user_add(*this);
	m->deep_copy(*this);
	return m;
}

template <typename R>
void Channel_user_add<R>
::_add_noise(const float *CP, const R *X_N, R *Y_N, const size_t frame_id)
{
	if (this->add_users && this->n_frames > 1) // n_frames_per_wave = n_frames
	{
		this->set_noise(0);

		std::copy(this->noised_data.data(), this->noised_data.data() + this->N, Y_N);

		std::fill(Y_N, Y_N + this->N, (R) 0);
		for (size_t f = 0; f < this->n_frames; f++)
			for (auto i = 0; i < this->N; i++)
				Y_N[i] += X_N[f * this->N + i];
	}
	else // n_frames_per_wave = 1
	{
		this->set_noise(frame_id);
		for (auto i = 0; i < this->N; i++)
			Y_N[i] = X_N[i] + this->noised_data[frame_id * this->N + i];
	}
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Channel_user_add<R_32>;
template class aff3ct::module::Channel_user_add<R_64>;
#else
template class aff3ct::module::Channel_user_add<R>;
#endif
// ==================================================================================== explicit template instantiation
