#include <algorithm>
#include <sstream>
#include <string>
#include <cmath>

#include "Tools/Noise/Noise.hpp"
#include "Tools/Algo/Draw_generator/Gaussian_noise_generator/Standard/Gaussian_noise_generator_std.hpp"
#include "Tools/Algo/Draw_generator/Gaussian_noise_generator/Fast/Gaussian_noise_generator_fast.hpp"
#ifdef AFF3CT_CHANNEL_GSL
#include "Tools/Algo/Draw_generator/Gaussian_noise_generator/GSL/Gaussian_noise_generator_GSL.hpp"
#endif
#ifdef AFF3CT_CHANNEL_MKL
#include "Tools/Algo/Draw_generator/Gaussian_noise_generator/MKL/Gaussian_noise_generator_MKL.hpp"
#endif
#include "Tools/Exception/exception.hpp"
#include "Module/Channel/Rayleigh/Channel_Rayleigh_LLR.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename R>
Channel_Rayleigh_LLR<R>
::Channel_Rayleigh_LLR(const int N,
                       const bool complex,
                       const tools::Gaussian_gen<R> &gaussian_generator,
                       const bool add_users)
: Channel<R>(N),
  complex(complex),
  add_users(add_users),
  gains(complex ? N * this->n_frames : 2 * N * this->n_frames),
  gaussian_generator(gaussian_generator.clone())
{
	const std::string name = "Channel_Rayleigh_LLR";
	this->set_name(name);

	if (complex && (N % 2))
	{
		std::stringstream message;
		message << "'N' has to be divisible by 2 ('N' = " << N << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (add_users)
		this->set_single_wave(true);
}

template <typename R>
tools::Gaussian_gen<R>* create_gaussian_generator(const tools::Gaussian_noise_generator_implem implem, const int seed)
{
	switch (implem)
	{
		case tools::Gaussian_noise_generator_implem::STD:
			return new tools::Gaussian_noise_generator_std<R>(seed);
			break;
		case tools::Gaussian_noise_generator_implem::FAST:
			return new tools::Gaussian_noise_generator_fast<R>(seed);
			break;
#ifdef AFF3CT_CHANNEL_GSL
		case tools::Gaussian_noise_generator_implem::GSL:
			return new tools::Gaussian_noise_generator_GSL<R>(seed);
			break;
#endif
#ifdef AFF3CT_CHANNEL_MKL
		case tools::Gaussian_noise_generator_implem::MKL:
			return new tools::Gaussian_noise_generator_MKL<R>(seed);
			break;
#endif
		default:
			std::stringstream message;
			message << "Unsupported 'implem' ('implem' = " << (int)implem << ").";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	};
}

template <typename R>
Channel_Rayleigh_LLR<R>
::Channel_Rayleigh_LLR(const int N,
                       const bool complex,
                       const tools::Gaussian_noise_generator_implem implem,
                       const int seed,
                       const bool add_users)
: Channel<R>(N),
  complex(complex),
  add_users(add_users),
  gains(complex ? N * this->n_frames : 2 * N * this->n_frames),
  gaussian_generator(create_gaussian_generator<R>(implem, seed))
{
	const std::string name = "Channel_Rayleigh_LLR";
	this->set_name(name);

	if (complex && (N % 2))
	{
		std::stringstream message;
		message << "'N' has to be divisible by 2 ('N' = " << N << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (add_users)
		this->set_single_wave(true);
}

template <typename R>
Channel_Rayleigh_LLR<R>* Channel_Rayleigh_LLR<R>
::clone() const
{
	auto m = new Channel_Rayleigh_LLR(*this);
	m->deep_copy(*this);
	return m;
}

template <typename R>
void Channel_Rayleigh_LLR<R>
::deep_copy(const Channel_Rayleigh_LLR<R> &m)
{
	Module::deep_copy(m);
	if (m.gaussian_generator != nullptr) this->gaussian_generator.reset(m.gaussian_generator->clone());
}

template <typename R>
void Channel_Rayleigh_LLR<R>
::set_seed(const int seed)
{
	this->gaussian_generator->set_seed(seed);
}

template <typename R>
void Channel_Rayleigh_LLR<R>
::_add_noise_wg(const float *CP, const R *X_N, R *H_N, R *Y_N, const size_t frame_id)
{
	if (add_users && this->n_frames > 1) // n_frames_per_wave = n_frames
	{
		gaussian_generator->generate(this->gains, (R)1 / (R)std::sqrt((R)2));
		gaussian_generator->generate(this->noised_data.data(), this->N, (R)*CP);

		std::fill(Y_N, Y_N + this->N, (R)0);

		if (this->complex)
		{
			for (size_t f = 0; f < this->n_frames; f++)
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
			for (size_t f = 0; f < this->n_frames; f++)
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
			Y_N[i] += this->noised_data[i];
	}
	else // n_frames_per_wave = 1
	{
		const auto gains_size = this->complex ? this->N : 2 * this->N;
		gaussian_generator->generate(this->gains      .data() + frame_id * this->N, gains_size, (R)1 / (R)std::sqrt((R)2));
		gaussian_generator->generate(this->noised_data.data() + frame_id * this->N, this->N, (R)*CP);

		if (this->complex)
		{
			for (auto n = 0; n < this->N; n += 2)
			{
				const auto h_re = H_N[n   ] = this->gains[frame_id * this->N + n   ];
				const auto h_im = H_N[n +1] = this->gains[frame_id * this->N + n +1];

				const auto n_re = this->noised_data[frame_id * this->N + n   ];
				const auto n_im = this->noised_data[frame_id * this->N + n +1];

				Y_N[n   ] = (X_N[n   ] * h_re - X_N[n +1] * h_im) + n_re;
				Y_N[n +1] = (X_N[n +1] * h_re + X_N[n   ] * h_im) + n_im;
			}
		}
		else
		{
			for (auto n = 0; n < this->N; n++)
			{
				const auto h_re = this->gains[frame_id * this->N + 2*n   ];
				const auto h_im = this->gains[frame_id * this->N + 2*n +1];

				H_N[n] = std::sqrt(h_re * h_re + h_im * h_im);
				Y_N[n] = X_N[n] * H_N[n] + this->noised_data[frame_id * this->N + n];
			}
		}
	}
}

template<typename R>
void Channel_Rayleigh_LLR<R>
::set_n_frames(const size_t n_frames)
{
	const auto old_n_frames = this->get_n_frames();
	if (old_n_frames != n_frames)
	{
		Channel<R>::set_n_frames(n_frames);

		const auto old_gains_size = this->gains.size();
		const auto new_gains_size = (old_gains_size / old_n_frames) * n_frames;
		this->gains.resize(new_gains_size);
	}
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Channel_Rayleigh_LLR<R_32>;
template class aff3ct::module::Channel_Rayleigh_LLR<R_64>;
#else
template class aff3ct::module::Channel_Rayleigh_LLR<R>;
#endif
// ==================================================================================== explicit template instantiation
