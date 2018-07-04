#include <cmath>
#include <sstream>

#include "Tools/Exception/exception.hpp"

#include "Channel_Rayleigh_LLR.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename R>
Channel_Rayleigh_LLR<R>
::Channel_Rayleigh_LLR(const int N, const bool complex, tools::Gaussian_gen<R> *noise_generator, const bool add_users,
                       const tools::Noise<R>& noise, const int n_frames)
: Channel<R>(N, noise, n_frames),
  complex(complex),
  add_users(add_users),
  gains(complex ? N * n_frames : 2 * N * n_frames),
  noise_generator(noise_generator)
{
	const std::string name = "Channel_Rayleigh_LLR";
	this->set_name(name);

	if (complex && N % 2)
	{
		std::stringstream message;
		message << "'N' has to be divisible by 2 ('N' = " << N << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (noise_generator == nullptr)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "'noise_generator' can't be NULL.");
}

template <typename R>
Channel_Rayleigh_LLR<R>
::Channel_Rayleigh_LLR(const int N, const bool complex, const int seed, const bool add_users,
                       const tools::Noise<R>& noise, const int n_frames)
: Channel<R>(N, noise, n_frames),
  complex(complex),
  add_users(add_users),
  gains(complex ? N * n_frames : 2 * N * n_frames),
  noise_generator(new tools::Gaussian_noise_generator_std<R>(seed))
{
	const std::string name = "Channel_Rayleigh_LLR";
	this->set_name(name);

	if (complex && N % 2)
	{
		std::stringstream message;
		message << "'N' has to be divisible by 2 ('N' = " << N << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename R>
Channel_Rayleigh_LLR<R>
::~Channel_Rayleigh_LLR()
{
	delete noise_generator;
}

template <typename R>
void Channel_Rayleigh_LLR<R>
::add_noise_wg(const R *X_N, R *H_N, R *Y_N, const int frame_id)
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

		noise_generator->generate(this->gains, (R)1 / (R)std::sqrt((R)2));
		noise_generator->generate(this->noise.data(), this->N, this->n->get_noise());

		std::fill(Y_N, Y_N + this->N, (R)0);

		if (this->complex)
		{
			for (auto f = 0; f < this->n_frames; f++)
			{
				for (auto i = 0; i < this->N; i += 2)
				{
					const auto h_re = H_N[f * this->N +i   ] = this->gains[f * this->N +i   ];
					const auto h_im = H_N[f * this->N +i +1] = this->gains[f * this->N +i +1];

					Y_N[i   ] += X_N[f * this->N +i   ] * h_re - X_N[f * this->N +i +1] * h_im;
					Y_N[i +1] += X_N[f * this->N +i +1] * h_re + X_N[f * this->N +i   ] * h_im;
				}
			}
		}
		else
		{
			for (auto f = 0; f < this->n_frames; f++)
			{
				for (auto i = 0; i < this->N; i++)
				{
					const auto h_re = this->gains[f * this->N * 2 + 2*i   ];
					const auto h_im = this->gains[f * this->N * 2 + 2*i +1];

					H_N[f * this->N +i] = std::sqrt(h_re * h_re + h_im * h_im);
					Y_N[i] += X_N[f * this->N +i] * H_N[f * this->N +i];
				}
			}
		}
		for (auto i = 0; i < this->N; i++)
			Y_N[i] += this->noise[i];
	}
	else
	{
		const auto f_start = (frame_id < 0) ? 0 : frame_id % this->n_frames;
		const auto f_stop  = (frame_id < 0) ? this->n_frames : f_start +1;

		if (frame_id < 0)
		{
			noise_generator->generate(this->gains, (R)1 / (R)std::sqrt((R)2));
			noise_generator->generate(this->noise, this->n->get_noise());
		}
		else
		{
			noise_generator->generate(this->gains.data() + f_start * this->N, this->N, (R)1 / (R)std::sqrt((R)2));
			noise_generator->generate(this->noise.data() + f_start * this->N, this->N, this->n->get_noise());
		}

		if (this->complex)
		{
			for (auto f = f_start; f < f_stop; f++)
			{
				for (auto n = 0; n < this->N; n += 2)
				{
					const auto h_re = H_N[f * this->N + n   ] = this->gains[f * this->N + n   ];
					const auto h_im = H_N[f * this->N + n +1] = this->gains[f * this->N + n +1];

					const auto n_re = this->noise[f * this->N + n   ];
					const auto n_im = this->noise[f * this->N + n +1];

					Y_N[f * this->N + n   ] = (X_N[f * this->N + n   ] * h_re - X_N[f * this->N + n +1] * h_im) + n_re;
					Y_N[f * this->N + n +1] = (X_N[f * this->N + n +1] * h_re + X_N[f * this->N + n   ] * h_im) + n_im;
				}
			}
		}
		else
		{
			for (auto f = f_start; f < f_stop; f++)
			{
				for (auto n = 0; n < this->N; n++)
				{
					const auto h_re = this->gains[f * this->N + 2*n   ];
					const auto h_im = this->gains[f * this->N + 2*n +1];

					H_N[f * this->N + n] = std::sqrt(h_re * h_re + h_im * h_im);
					Y_N[f * this->N + n] = X_N[f * this->N + n] * H_N[f * this->N + n] + this->noise[f * this->N + n];
				}
			}
		}
	}
}

template<typename R>
void Channel_Rayleigh_LLR<R>::check_noise()
{
	Channel<R>::check_noise();

	this->n->is_of_type_throw(tools::Noise_type::SIGMA);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Channel_Rayleigh_LLR<R_32>;
template class aff3ct::module::Channel_Rayleigh_LLR<R_64>;
#else
template class aff3ct::module::Channel_Rayleigh_LLR<R>;
#endif
// ==================================================================================== explicit template instantiation
