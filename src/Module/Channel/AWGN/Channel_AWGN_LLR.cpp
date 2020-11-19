#include <algorithm>
#include <string>

#include "Tools/Noise/Noise.hpp"
#include "Tools/Exception/exception.hpp"
#include "Tools/Algo/Draw_generator/Gaussian_noise_generator/Standard/Gaussian_noise_generator_std.hpp"
#include "Tools/Algo/Draw_generator/Gaussian_noise_generator/Fast/Gaussian_noise_generator_fast.hpp"
#ifdef AFF3CT_CHANNEL_GSL
#include "Tools/Algo/Draw_generator/Gaussian_noise_generator/GSL/Gaussian_noise_generator_GSL.hpp"
#endif
#ifdef AFF3CT_CHANNEL_MKL
#include "Tools/Algo/Draw_generator/Gaussian_noise_generator/MKL/Gaussian_noise_generator_MKL.hpp"
#endif
#include "Module/Channel/AWGN/Channel_AWGN_LLR.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename R>
Channel_AWGN_LLR<R>
::Channel_AWGN_LLR(const int N,
                   const tools::Gaussian_gen<R> &gaussian_generator,
                   const bool add_users)
: Channel<R>(N),
  add_users(add_users),
  gaussian_generator(gaussian_generator.clone())
{
	const std::string name = "Channel_AWGN_LLR";
	this->set_name(name);

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
Channel_AWGN_LLR<R>
::Channel_AWGN_LLR(const int N,
                   const tools::Gaussian_noise_generator_implem implem,
                   const int seed,
                   const bool add_users)
: Channel<R>(N),
  add_users(add_users),
  gaussian_generator(create_gaussian_generator<R>(implem, seed))
{
	const std::string name = "Channel_AWGN_LLR";
	this->set_name(name);

	if (add_users)
		this->set_single_wave(true);
}

template <typename R>
Channel_AWGN_LLR<R>* Channel_AWGN_LLR<R>
::clone() const
{
	auto m = new Channel_AWGN_LLR(*this);
	m->deep_copy(*this);
	return m;
}

template <typename R>
void Channel_AWGN_LLR<R>
::deep_copy(const Channel_AWGN_LLR<R> &m)
{
	Module::deep_copy(m);
	if (m.gaussian_generator != nullptr) this->gaussian_generator.reset(m.gaussian_generator->clone());
}

template <typename R>
void Channel_AWGN_LLR<R>
::set_seed(const int seed)
{
	this->gaussian_generator->set_seed(seed);
}

template <typename R>
void Channel_AWGN_LLR<R>
::_add_noise(const float *CP, const R *X_N, R *Y_N, const size_t frame_id)
{
	if (add_users && this->n_frames > 1) // n_frames_per_wave = n_frames
	{
		gaussian_generator->generate(this->noised_data.data(), this->N, (R)*CP);

		std::fill(Y_N, Y_N + this->N, (R)0);
		for (size_t f = 0; f < this->n_frames; f++)
			for (auto i = 0; i < this->N; i++)
				Y_N[i] += X_N[f * this->N +i];

		for (auto i = 0; i < this->N; i++)
			Y_N[i] += this->noised_data[i];
	}
	else // n_frames_per_wave = 1
	{
		gaussian_generator->generate(this->noised_data.data() + frame_id * this->N, this->N, (R)*CP);

		for (auto n = 0; n < this->N; n++)
			Y_N[n] = X_N[n] + this->noised_data[frame_id * this->N +n];
	}
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
