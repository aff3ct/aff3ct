#include <cmath>
#include <fstream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Algo/Draw_generator/Gaussian_noise_generator/Standard/Gaussian_noise_generator_std.hpp"

#include "Channel_Rayleigh_LLR_user.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename R>
Channel_Rayleigh_LLR_user<R>
::Channel_Rayleigh_LLR_user(const int N, const bool complex, const std::string& gains_filename,
                            std::unique_ptr<tools::Gaussian_gen<R>>&& _ng, const int gain_occurrences,
                            const bool add_users, const tools::Noise<R>& noise, const int n_frames)
: Channel<R>(N, noise, n_frames),
  complex(complex),
  add_users(add_users),
  gains(N * n_frames),
  noise_generator(std::move(_ng)),
  gain_occur(gain_occurrences),
  current_gain_occur(0),
  gain_index(0)
{
	const std::string name = "Channel_Rayleigh_LLR_user";
	this->set_name(name);

	if (complex || add_users)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Arguments 'complex' and 'add_users' are not supported yet.");

	if (noise_generator == nullptr)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "'noise_generator' can't be NULL.");

	if (gain_occurrences <= 0)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Argument 'gain_occurrences' must be strictly positive.");

	read_gains(gains_filename);
}

template <typename R>
Channel_Rayleigh_LLR_user<R>
::Channel_Rayleigh_LLR_user(const int N, const bool complex, const int seed, const std::string& gains_filename,
                            const int gain_occurrences, const bool add_users, const tools::Noise<R>& noise, const int n_frames)
: Channel<R>(N, noise, n_frames),
  complex(complex),
  add_users(add_users),
  gains(N * n_frames),
  noise_generator(new tools::Gaussian_noise_generator_std<R>(seed)),
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
::add_noise_wg(const R *X_N, R *H_N, R *Y_N, const int frame_id)
{
	this->check_noise();

	if (frame_id != -1)
	{
		std::stringstream message;
		message << "'frame_id' has to be equal to -1 ('frame_id' = " << frame_id << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

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
	noise_generator->generate(this->noise, this->n->get_noise()); // trow if noise is not SIGMA type

	// use the noise and the gain to modify the signal
	for (auto i = 0; i < this->N * this->n_frames; i++)
	{
		H_N[i] = this->gains[i];

		Y_N[i] = X_N[i] * H_N[i] + this->noise[i];
	}
}

template<typename R>
void Channel_Rayleigh_LLR_user<R>::check_noise()
{
	Channel<R>::check_noise();

	this->n->is_of_type_throw(tools::Noise_type::SIGMA);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Channel_Rayleigh_LLR_user<R_32>;
template class aff3ct::module::Channel_Rayleigh_LLR_user<R_64>;
#else
template class aff3ct::module::Channel_Rayleigh_LLR_user<R>;
#endif
// ==================================================================================== explicit template instantiation
