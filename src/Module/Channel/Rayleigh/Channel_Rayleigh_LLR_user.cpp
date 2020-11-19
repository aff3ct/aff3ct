#include <fstream>
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
#include "Tools/Algo/Draw_generator/Gaussian_noise_generator/Standard/Gaussian_noise_generator_std.hpp"
#include "Module/Channel/Rayleigh/Channel_Rayleigh_LLR_user.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename R>
Channel_Rayleigh_LLR_user<R>
::Channel_Rayleigh_LLR_user(const int N,
                            const bool complex,
                            const tools::Gaussian_gen<R> &gaussian_generator,
                            const std::string& gains_filename,
                            const int gain_occurrences,
                            const bool add_users)
: Channel<R>(N),
  complex(complex),
  add_users(add_users),
  gains(N * this->n_frames),
  gaussian_generator(gaussian_generator.clone()),
  gain_occur(gain_occurrences),
  current_gain_occur(0),
  gain_index(0)
{
	const std::string name = "Channel_Rayleigh_LLR_user";
	this->set_name(name);

	if (complex || add_users)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Arguments 'complex' and 'add_users' are not supported yet.");

	if (gain_occurrences <= 0)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Argument 'gain_occurrences' must be strictly positive.");

	read_gains(gains_filename);

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
Channel_Rayleigh_LLR_user<R>
::Channel_Rayleigh_LLR_user(const int N,
                            const bool complex,
                            const std::string& gains_filename,
                            const tools::Gaussian_noise_generator_implem implem,
                            const int seed,
                            const int gain_occurrences,
                            const bool add_users)
: Channel<R>(N),
  complex(complex),
  add_users(add_users),
  gains(N * this->n_frames),
  gaussian_generator(create_gaussian_generator<R>(implem, seed)),
  gain_occur(gain_occurrences),
  current_gain_occur(0),
  gain_index(0)
{
	const std::string name = "Channel_Rayleigh_LLR_user";
	this->set_name(name);

	if (complex || add_users)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Arguments 'complex' and 'add_users' are not supported yet.");

	if (gain_occurrences <= 0)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Argument 'gain_occurrences' must be strictly positive.");

	read_gains(gains_filename);

	this->set_single_wave(true);
}

template <typename R>
Channel_Rayleigh_LLR_user<R>* Channel_Rayleigh_LLR_user<R>
::clone() const
{
	auto m = new Channel_Rayleigh_LLR_user(*this);
	m->deep_copy(*this);
	return m;
}

template <typename R>
void Channel_Rayleigh_LLR_user<R>
::deep_copy(const Channel_Rayleigh_LLR_user<R> &m)
{
	Module::deep_copy(m);
	if (m.gaussian_generator != nullptr) this->gaussian_generator.reset(m.gaussian_generator->clone());
}

template <typename R>
void Channel_Rayleigh_LLR_user<R>
::set_seed(const int seed)
{
	this->gaussian_generator->set_seed(seed);
}

template <typename R>
void Channel_Rayleigh_LLR_user<R>
::read_gains(const std::string& gains_filename)
{
	if (gains_filename.empty())
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Argument 'gains_filename' should not be empty.");

	std::ifstream file(gains_filename);

	if (file.is_open())
	{
		R val;
		while (!file.eof() && !file.bad())
		{
			file >> val;
			gains_stock.push_back(val);
		}

		file.close();
	}
	else
	{
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Can't open '" + gains_filename + "' file.");
	}

	if(gains_stock.empty())
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "The file '" + gains_filename + "' is empty.");
}

template <typename R>
void Channel_Rayleigh_LLR_user<R>
::_add_noise_wg(const float *CP, const R *X_N, R *H_N, R *Y_N, const size_t frame_id)
{
	// get all the needed gains from the stock
	for (unsigned i = 0; i < gains.size(); ++i)
	{
		gains[i] = gains_stock[gain_index];

		current_gain_occur++;
		if(current_gain_occur >= gain_occur)
		{
			current_gain_occur = 0;
			gain_index++;

			if(gain_index >= gains_stock.size())
				gain_index = 0;
		}
	}

	// generate the noise
	gaussian_generator->generate(this->noised_data, (R)*CP);

	// use the noise and the gain to modify the signal
	for (size_t i = 0; i < this->N * this->n_frames; i++)
	{
		H_N[i] = this->gains[i];
		Y_N[i] = X_N[i] * H_N[i] + this->noised_data[i];
	}
}

template<typename R>
void Channel_Rayleigh_LLR_user<R>
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
template class aff3ct::module::Channel_Rayleigh_LLR_user<R_32>;
template class aff3ct::module::Channel_Rayleigh_LLR_user<R_64>;
#else
template class aff3ct::module::Channel_Rayleigh_LLR_user<R>;
#endif
// ==================================================================================== explicit template instantiation
