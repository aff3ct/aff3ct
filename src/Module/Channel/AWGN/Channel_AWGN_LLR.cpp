#include <algorithm>

#include "Tools/Exception/exception.hpp"
#include "Tools/Algo/Draw_generator/Gaussian_noise_generator/Standard/Gaussian_noise_generator_std.hpp"

#include "Channel_AWGN_LLR.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename R>
Channel_AWGN_LLR<R>
::Channel_AWGN_LLR(const int N, std::unique_ptr<tools::Gaussian_gen<R>>&& _ng, const bool add_users,
                   const tools::Sigma<R>& noise, const int n_frames)
: Channel<R>(N, noise, n_frames),
  add_users(add_users),
  noise_generator(std::move(_ng))
{
	const std::string name = "Channel_AWGN_LLR";
	this->set_name(name);

	if (this->noise_generator == nullptr)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "'noise_generator' can't be NULL.");
}

template <typename R>
Channel_AWGN_LLR<R>
::Channel_AWGN_LLR(const int N, const int seed, const bool add_users, const tools::Sigma<R>& noise, const int n_frames)
: Channel_AWGN_LLR<R>(N, std::unique_ptr<tools::Gaussian_noise_generator_std<R>>(new tools::Gaussian_noise_generator_std<R>(seed)),
  add_users, noise, n_frames)
{
}

template <typename R>
void Channel_AWGN_LLR<R>
::add_noise(const R *X_N, R *Y_N, const int frame_id)
{
	this->check_noise();

	if (add_users && this->n_frames > 1)
	{
		if (frame_id != -1)
		{
			std::stringstream message;
			message << "'frame_id' has to be equal to -1 ('frame_id' = " << frame_id << ").";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}

		noise_generator->generate(this->noise.data(), this->N, this->n->get_noise());

		std::fill(Y_N, Y_N + this->N, (R)0);
		for (auto f = 0; f < this->n_frames; f++)
			for (auto i = 0; i < this->N; i++)
				Y_N[i] += X_N[f * this->N +i];

		for (auto i = 0; i < this->N; i++)
			Y_N[i] += this->noise[i];
	}
	else
	{
		const auto f_start = (frame_id < 0) ? 0 : frame_id % this->n_frames;
		const auto f_stop  = (frame_id < 0) ? this->n_frames : f_start +1;

		if (frame_id < 0)
			noise_generator->generate(this->noise, this->n->get_noise());
		else
			noise_generator->generate(this->noise.data() + f_start * this->N, this->N, this->n->get_noise());

		for (auto f = f_start; f < f_stop; f++)
			for (auto n = 0; n < this->N; n++)
				Y_N[f * this->N +n] = X_N[f * this->N +n] + this->noise[f * this->N +n];
	}
}

template<typename R>
void Channel_AWGN_LLR<R>::check_noise()
{
	Channel<R>::check_noise();

	this->n->is_of_type_throw(tools::Noise_type::SIGMA);
}
// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Channel_AWGN_LLR<R_32>;
template class aff3ct::module::Channel_AWGN_LLR<R_64>;
#else
template class aff3ct::module::Channel_AWGN_LLR<R>;
#endif
// ==================================================================================== explicit template instantiation
