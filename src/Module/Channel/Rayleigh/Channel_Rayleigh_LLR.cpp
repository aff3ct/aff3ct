#include <cmath>
#include <stdexcept>

#include "Channel_Rayleigh_LLR.hpp"

using namespace aff3ct::module;

template <typename R>
Channel_Rayleigh_LLR<R>
::Channel_Rayleigh_LLR(const int N, const R sigma, const bool complex, tools::Noise<R> *noise_generator,
                       const int n_frames, const std::string name)
: Channel<R>(N, sigma, n_frames, name),
  complex(complex),
  gains(complex ? N * n_frames : 2 * N * n_frames),
  noise_generator(noise_generator)
{
	if (complex && N % 2)
		throw std::invalid_argument("aff3ct::module::Channel_Rayleigh_LLR: \"N\" has to be divisible by 2.");

	if (noise_generator == nullptr)
		throw std::invalid_argument("aff3ct::module::Channel_Rayleigh_LLR: \"noise_generator\" can't be NULL.");
}

template <typename R>
Channel_Rayleigh_LLR<R>
::~Channel_Rayleigh_LLR()
{
	delete noise_generator;
}

template <typename R>
void Channel_Rayleigh_LLR<R>
::add_noise(const R *X_N, R *Y_N, R *H_N)
{
	noise_generator->generate(this->gains, (R)1 / (R)std::sqrt((R)2));
	noise_generator->generate(this->noise, this->sigma);

	if (this->complex)
	{
		for (auto i = 0; i < (this->N * this->n_frames) / 2 ; i++)
		{
			const auto h_re = this->gains[2*i   ];
			const auto h_im = this->gains[2*i +1];

			const auto n_re = this->noise[2*i   ];
			const auto n_im = this->noise[2*i +1];

			Y_N[2*i   ] = h_re * X_N[2*i   ] - h_im * X_N[2*i +1] + n_re;
			Y_N[2*i +1] = h_re * X_N[2*i +1] + h_im * X_N[2*i   ] + n_im;
		}
	}
	else
	{
		for (auto i = 0; i < this->N * this->n_frames ; i++)
		{
			const auto h_re = this->gains[2*i   ];
			const auto h_im = this->gains[2*i +1];

			H_N[i] = std::sqrt(h_re * h_re + h_im * h_im);
			Y_N[i] = H_N[i] * X_N[i] + this->noise[i];
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
