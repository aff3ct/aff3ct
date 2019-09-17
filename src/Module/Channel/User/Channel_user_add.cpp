#include <algorithm>
#include <sstream>
#include <string>

#include "Tools/Exception/exception.hpp"
#include "Module/Channel/User/Channel_user_add.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename R>
Channel_user_add<R>
::Channel_user_add(const int N, const std::string &filename, const bool add_users, const int n_frames)
: Channel_user<R>(N, filename, add_users, n_frames)
{
	const std::string name = "Channel_user_add";
	this->set_name(name);
}

template <typename R>
void Channel_user_add<R>
::add_noise(const R *X_N, R *Y_N, const int frame_id)
{
	if (this->add_users && this->n_frames > 1)
	{
		if (frame_id != -1)
		{
			std::stringstream message;
			message << "'frame_id' has to be equal to -1 ('frame_id' = " << frame_id << ").";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}

		this->set_noise(0);

		std::copy(this->noise.data(), this->noise.data() + this->N, Y_N);

		std::fill(Y_N, Y_N + this->N, (R) 0);
		for (auto f = 0; f < this->n_frames; f++)
			for (auto i = 0; i < this->N; i++)
				Y_N[i] += X_N[f * this->N + i];
	}
	else
	{
		const auto f_start = (frame_id < 0) ? 0 : frame_id % this->n_frames;
		const auto f_stop  = (frame_id < 0) ? this->n_frames : f_start + 1;

		for (auto f = f_start; f < f_stop; f++)
		{
			this->set_noise(f);

			for (auto i = 0; i < this->N; i++)
				Y_N[f * this->N + i] = X_N[f * this->N + i] + this->noise[f * this->N + i];
		}
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
