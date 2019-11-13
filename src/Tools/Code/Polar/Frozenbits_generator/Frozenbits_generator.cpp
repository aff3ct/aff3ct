#include <algorithm>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <numeric>
#include <ios>

#include "Tools/Noise/noise_utils.h"
#include "Tools/Exception/exception.hpp"
#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

std::thread::id Frozenbits_generator::master_thread_id = std::this_thread::get_id();

Frozenbits_generator
::Frozenbits_generator(const int K, const int N, const std::string &dump_channels_path, const bool dump_channels_single_thread)
: dump_channels_path(dump_channels_path),
  dump_channels_single_thread(dump_channels_single_thread),
  K(K),
  N(N),
  noise(nullptr),
  best_channels(N)
{
	std::iota(this->best_channels.begin(), this->best_channels.end(), 0);
}

int Frozenbits_generator
::get_K() const
{
	return this->K;
}

int Frozenbits_generator
::get_N() const
{
	return this->N;
}

void Frozenbits_generator
::set_noise(const tools::Noise<>& noise)
{
	this->noise = &noise;
}

const tools::Noise<>& Frozenbits_generator
::get_noise() const
{
	if (this->noise == nullptr)
	{
		std::stringstream message;
		message << "'noise' should not be nullptr.";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	return *this->noise;
}

void Frozenbits_generator
::generate(std::vector<bool> &frozen_bits)
{
	if (frozen_bits.size() != (unsigned)N)
	{
		std::stringstream message;
		message << "'frozen_bits.size()' has to be equal to 'N' ('frozen_bits.size()' = " << frozen_bits.size()
		        << ", 'N' = " << N << ").";
		throw length_error(__FILE__, __LINE__, __func__, message.str());
	}

	this->evaluate();

	// init frozen_bits vector, true means frozen bits, false means information bits
	std::fill(frozen_bits.begin(), frozen_bits.end(), true);
	for (auto i = 0; i < K; i++)
		frozen_bits[best_channels[i]] = false;

	if (!dump_channels_path.empty() && (!dump_channels_single_thread || this->master_thread_id == std::this_thread::get_id()))
	{
		std::string noise_type = "unkn";
		if (this->noise != nullptr)
		{
			switch (this->noise->get_type())
			{
				case tools::Noise_type::SIGMA:
					noise_type = "awgn";
					break;
				case tools::Noise_type::EP:
					noise_type = "bec";
					break;
				default:
					break;
			};
		}

		std::stringstream noise;
		noise << std::fixed << std::setprecision(3) << this->noise->get_value();

		std::string dump_channels_full_path = dump_channels_path + "/N" + std::to_string(this->N) + "_"
		                                                         + noise_type + "_s" + noise.str() + ".pc";
		this->dump_best_channels(dump_channels_full_path);
	}
}

const std::vector<uint32_t>& Frozenbits_generator
::get_best_channels() const
{
	return best_channels;
}

void Frozenbits_generator
::check_noise()
{
	if (this->noise == nullptr)
	{
		std::stringstream message;
		message << "'noise' should not be nullptr.";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}
	if (!this->noise->is_set())
	{
		throw tools::runtime_error(__FILE__, __LINE__, __func__, "'noise' is not set.");
	}
}

void Frozenbits_generator
::dump_best_channels(const std::string& dump_channels_full_path) const
{
	std::string noise_type = "unkn";
	if (this->noise != nullptr)
	{
		switch (this->noise->get_type())
		{
			case tools::Noise_type::SIGMA:
				noise_type = "awgn";
				break;
			case tools::Noise_type::EP:
				noise_type = "bec";
				break;
			default:
				break;
		};
	}

	std::ofstream file(dump_channels_full_path);

	if (!file.is_open())
	{
		std::stringstream message;
		message << "Impossible to open the '" << dump_channels_full_path << "' file to dump the Polar best channels.";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	file << this->N << std::endl;
	file << noise_type << std::endl;
	file << std::fixed << std::setprecision(3) << this->noise->get_value() << std::endl;

	for (auto c : this->best_channels)
		file << c << " ";
	file << std::endl;
}