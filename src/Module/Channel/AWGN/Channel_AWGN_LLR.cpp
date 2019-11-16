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
                   const bool add_users,
                   const int n_frames)
: Channel<R>(N, n_frames),
  add_users(add_users),
  gaussian_generator(gaussian_generator.clone())
{
	const std::string name = "Channel_AWGN_LLR";
	this->set_name(name);
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
                   const bool add_users,
                   const int n_frames)
: Channel<R>(N, n_frames),
  add_users(add_users),
  gaussian_generator(create_gaussian_generator<R>(implem, seed))
{
	const std::string name = "Channel_AWGN_LLR";
	this->set_name(name);
}

template <typename R>
Channel_AWGN_LLR<R>* Channel_AWGN_LLR<R>
::clone() const
{
	auto m = new Channel_AWGN_LLR<R>(*this); // soft copy constructor
	m->deep_copy(*this); // hard copy
	return m;
}

template <typename R>
void Channel_AWGN_LLR<R>
::deep_copy(const Channel_AWGN_LLR<R> &m)
{
	Module::deep_copy(m);
	this->gaussian_generator.reset(m.gaussian_generator->clone());
}

template <typename R>
void Channel_AWGN_LLR<R>
::set_seed(const int seed)
{
	this->gaussian_generator->set_seed(seed);
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

		gaussian_generator->generate(this->noised_data.data(), this->N, (R)this->noise->get_value());

		std::fill(Y_N, Y_N + this->N, (R)0);
		for (auto f = 0; f < this->n_frames; f++)
			for (auto i = 0; i < this->N; i++)
				Y_N[i] += X_N[f * this->N +i];

		for (auto i = 0; i < this->N; i++)
			Y_N[i] += this->noised_data[i];
	}
	else
	{
		const auto f_start = (frame_id < 0) ? 0 : frame_id % this->n_frames;
		const auto f_stop  = (frame_id < 0) ? this->n_frames : f_start +1;

		if (frame_id < 0)
			gaussian_generator->generate(this->noised_data, (R)this->noise->get_value());
		else
			gaussian_generator->generate(this->noised_data.data() + f_start * this->N, this->N, (R)this->noise->get_value());

		for (auto f = f_start; f < f_stop; f++)
			for (auto n = 0; n < this->N; n++)
				Y_N[f * this->N +n] = X_N[f * this->N +n] + this->noised_data[f * this->N +n];
	}
}

template<typename R>
void Channel_AWGN_LLR<R>::check_noise()
{
	Channel<R>::check_noise();
	this->noise->is_of_type_throw(tools::Noise_type::SIGMA);
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
