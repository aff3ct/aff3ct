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
                       tools::Gaussian_gen<R> &gaussian_generator,
                       const bool add_users,
                       const int n_frames)
: Channel<R>(N, n_frames),
  complex(complex),
  add_users(add_users),
  gains(complex ? N * n_frames : 2 * N * n_frames),
  gaussian_generator(&gaussian_generator),
  is_autoalloc_gaussian_gen(false)
{
	const std::string name = "Channel_Rayleigh_LLR";
	this->set_name(name);

	if (complex && (N % 2))
	{
		std::stringstream message;
		message << "'N' has to be divisible by 2 ('N' = " << N << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename R>
Channel_Rayleigh_LLR<R>
::Channel_Rayleigh_LLR(const int N,
                       const bool complex,
                       const tools::Gaussian_noise_generator_implem implem,
                       const int seed,
                       const bool add_users,
                       const int n_frames)
: Channel<R>(N, n_frames),
  complex(complex),
  add_users(add_users),
  gains(complex ? N * n_frames : 2 * N * n_frames),
  gaussian_generator(nullptr),
  is_autoalloc_gaussian_gen(true)
{
	const std::string name = "Channel_Rayleigh_LLR";
	this->set_name(name);

	if (complex && (N % 2))
	{
		std::stringstream message;
		message << "'N' has to be divisible by 2 ('N' = " << N << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	switch (implem)
	{
		case tools::Gaussian_noise_generator_implem::STD:
			this->gaussian_generator = new tools::Gaussian_noise_generator_std<R>(seed);
			break;
		case tools::Gaussian_noise_generator_implem::FAST:
			this->gaussian_generator = new tools::Gaussian_noise_generator_fast<R>(seed);
			break;
#ifdef AFF3CT_CHANNEL_GSL
		case tools::Gaussian_noise_generator_implem::GSL:
			this->gaussian_generator = new tools::Gaussian_noise_generator_GSL<R>(seed);
			break;
#endif
#ifdef AFF3CT_CHANNEL_MKL
		case tools::Gaussian_noise_generator_implem::MKL:
			this->gaussian_generator = new tools::Gaussian_noise_generator_MKL<R>(seed);
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
::~Channel_Rayleigh_LLR()
{
	if (this->is_autoalloc_gaussian_gen)
		delete gaussian_generator;
}

template <typename R>
void Channel_Rayleigh_LLR<R>
::set_seed(const int seed)
{
	this->gaussian_generator->set_seed(seed);
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

		gaussian_generator->generate(this->gains, (R)1 / (R)std::sqrt((R)2));
		gaussian_generator->generate(this->noised_data.data(), this->N, (R)this->noise->get_value());

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
			Y_N[i] += this->noised_data[i];
	}
	else
	{
		const auto f_start = (frame_id < 0) ? 0 : frame_id % this->n_frames;
		const auto f_stop  = (frame_id < 0) ? this->n_frames : f_start +1;

		if (frame_id < 0)
		{
			gaussian_generator->generate(this->gains, (R)1 / (R)std::sqrt((R)2));
			gaussian_generator->generate(this->noised_data, (R)this->noise->get_value());
		}
		else
		{
			gaussian_generator->generate(this->gains      .data() + f_start * this->N, this->N, (R)1 / (R)std::sqrt((R)2));
			gaussian_generator->generate(this->noised_data.data() + f_start * this->N, this->N, (R)this->noise->get_value());
		}

		if (this->complex)
		{
			for (auto f = f_start; f < f_stop; f++)
			{
				for (auto n = 0; n < this->N; n += 2)
				{
					const auto h_re = H_N[f * this->N + n   ] = this->gains[f * this->N + n   ];
					const auto h_im = H_N[f * this->N + n +1] = this->gains[f * this->N + n +1];

					const auto n_re = this->noised_data[f * this->N + n   ];
					const auto n_im = this->noised_data[f * this->N + n +1];

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
					Y_N[f * this->N + n] = X_N[f * this->N + n] * H_N[f * this->N + n] + this->noised_data[f * this->N + n];
				}
			}
		}
	}
}

template<typename R>
void Channel_Rayleigh_LLR<R>::check_noise()
{
	Channel<R>::check_noise();

	this->noise->is_of_type_throw(tools::Noise_type::SIGMA);
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
