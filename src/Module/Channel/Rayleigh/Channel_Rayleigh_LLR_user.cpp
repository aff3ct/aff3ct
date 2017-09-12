#include <cmath>
#include <fstream>

#include "Tools/Exception/exception.hpp"

#include "Channel_Rayleigh_LLR_user.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename R>
Channel_Rayleigh_LLR_user<R>
::Channel_Rayleigh_LLR_user(const int N, const bool complex, const std::string& gains_filename,
                            const int gain_occurrences, tools::Noise<R> *noise_generator, const bool add_users,
                            const R sigma, const int n_frames, const std::string name)
: Channel_Rayleigh_LLR<R>(N, complex, noise_generator, add_users, sigma, n_frames, name),
  gain_occur(gain_occurrences),
  current_gain_occur(0),
  gain_index(0)
{
	if (gain_occurrences <= 0)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Argument 'gain_occurrences' must be strictly positive.");

	read_gains(gains_filename);
}

template <typename R>
Channel_Rayleigh_LLR_user<R>
::Channel_Rayleigh_LLR_user(const int N, const bool complex, const int seed, const std::string& gains_filename,
                            const int gain_occurrences, const bool add_users, const R sigma,
                            const int n_frames, const std::string name)
: Channel_Rayleigh_LLR<R>(N, complex, seed, add_users, sigma, n_frames, name),
  gain_occur(gain_occurrences),
  current_gain_occur(0),
  gain_index(0)
{
	read_gains(gains_filename);
}

template <typename R>
Channel_Rayleigh_LLR_user<R>
::~Channel_Rayleigh_LLR_user()
{
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
::get_gains(std::vector<R>& gains, const R sigma)
{
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
