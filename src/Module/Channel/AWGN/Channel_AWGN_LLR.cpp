#include <algorithm>
#include <stdexcept>

#include "Channel_AWGN_LLR.hpp"

using namespace aff3ct::module;

template <typename R>
Channel_AWGN_LLR<R>
::Channel_AWGN_LLR(const int N, const R sigma, tools::Noise<R> *noise_generator, const bool add_users,
                   const int n_frames, const std::string name)
: Channel<R>(N, sigma, n_frames, name),
  add_users(add_users),
  noise_generator(noise_generator)
{
	if (noise_generator == nullptr)
		throw std::invalid_argument("aff3ct::module::Channel_AWGN_LLR: \"noise_generator\" can't be NULL.");
}

template <typename R>
Channel_AWGN_LLR<R>
::~Channel_AWGN_LLR()
{
	delete noise_generator;
}

template <typename R>
void Channel_AWGN_LLR<R>
::add_noise(const R *X_N, R *Y_N)
{
	if (add_users)
	{
		noise_generator->generate(this->noise.data(), this->N, this->sigma);

		std::fill(Y_N, Y_N + this->N, (R)0);
		for (auto f = 0; f < this->n_frames; f++)
			for (auto i = 0; i < this->N; i++)
				Y_N[i] += X_N[f * this->N +i];

		for (auto i = 0; i < this->N; i++)
			Y_N[i] += this->noise[i];
	}
	else
	{
		noise_generator->generate(this->noise, this->sigma);

		for (auto i = 0; i < this->N * this->n_frames; i++)
			Y_N[i] = X_N[i] + this->noise[i];
	}
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Channel_AWGN_LLR<R_32>;
template class aff3ct::module::Channel_AWGN_LLR<R_64>;
#else
template class aff3ct::module::Channel_AWGN_LLR<R>;
#endif
// ==================================================================================== explicit template instantiation
