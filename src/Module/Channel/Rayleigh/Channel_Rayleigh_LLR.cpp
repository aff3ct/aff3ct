#include <cmath>
#include <sstream>

#include "Tools/Exception/exception.hpp"

#include "Channel_Rayleigh_LLR.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename R>
Channel_Rayleigh_LLR<R>
::Channel_Rayleigh_LLR(const int N, const bool complex, tools::Noise<R> *noise_generator, const bool add_users,
                       const R sigma, const int n_frames, const std::string name)
: Channel<R>(N, sigma, n_frames, name),
  complex(complex),
  add_users(add_users),
  gains(complex ? N * n_frames : 2 * N * n_frames),
  noise_generator(noise_generator)
{
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
::Channel_Rayleigh_LLR(const int N, const bool complex, const int seed, const bool add_users, const R sigma,
                       const int n_frames, const std::string name)
: Channel<R>(N, sigma, n_frames, name),
  complex(complex),
  add_users(add_users),
  gains(complex ? N * n_frames : 2 * N * n_frames),
  noise_generator(new tools::Noise_std<R>(seed))
{
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
::get_gains(std::vector<R>& gains, const R sigma)
{
	noise_generator->generate(gains, sigma);
}

template <typename R>
void Channel_Rayleigh_LLR<R>
::add_noise_wg(const R *X_N, R *H_N, R *Y_N)
{
	if (add_users && this->n_frames > 1)
	{
		this->get_gains(this->gains, (R)1 / (R)std::sqrt((R)2));
		noise_generator->generate(this->noise.data(), this->N, this->sigma);

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
		this->get_gains(this->gains, (R)1 / (R)std::sqrt((R)2));
		noise_generator->generate(this->noise, this->sigma);

		if (this->complex)
		{
			for (auto i = 0; i < this->N * this->n_frames; i += 2)
			{
				const auto h_re = H_N[i   ] = this->gains[i   ];
				const auto h_im = H_N[i +1] = this->gains[i +1];

				const auto n_re = this->noise[i   ];
				const auto n_im = this->noise[i +1];

				Y_N[i   ] = (X_N[i   ] * h_re - X_N[i +1] * h_im) + n_re;
				Y_N[i +1] = (X_N[i +1] * h_re + X_N[i   ] * h_im) + n_im;
			}
		}
		else
		{
			for (auto i = 0; i < this->N * this->n_frames; i++)
			{
				const auto h_re = this->gains[2*i   ];
				const auto h_im = this->gains[2*i +1];

				H_N[i] = std::sqrt(h_re * h_re + h_im * h_im);
				Y_N[i] = X_N[i] * H_N[i] + this->noise[i];
			}
		}
	}
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
