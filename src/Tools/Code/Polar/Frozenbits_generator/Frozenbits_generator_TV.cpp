#ifdef _MSC_VER
#include <direct.h>
#else
#include <sys/stat.h>
#endif
#include <dirent.h>
#include <errno.h>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <mutex>
#include <ios>

#include "Tools/Exception/exception.hpp"
#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator_TV.hpp"

using namespace aff3ct::tools;

const int Frozenbits_generator_TV::Mu = 100;

Frozenbits_generator_TV
::Frozenbits_generator_TV(const int K, const int N,
                          const std::string &awgn_codes_dir,
                          const std::string &bin_pb_path)
: Frozenbits_generator_file(K, N), m((int)std::log2(N)), awgn_codes_dir(awgn_codes_dir), bin_pb_path(bin_pb_path)
{
}

Frozenbits_generator_TV* Frozenbits_generator_TV
::clone() const
{
	auto t = new Frozenbits_generator_TV(*this);
	return t;
}

void Frozenbits_generator_TV
::evaluate()
{
	this->check_noise();

	std::ostringstream s_stream;
	s_stream << std::setiosflags(std::ios::fixed) << std::setprecision(3) << this->noise->get_value();
	auto str_sigma = s_stream.str();
	auto str_N = std::to_string(this->N);
	auto str_m = std::to_string(m);

	DIR *dp;
	if ((dp = opendir(awgn_codes_dir.c_str())) == nullptr)
	{
		std::stringstream message;
		message << "The following directory does not exist: '" + awgn_codes_dir + "'.";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
	else
	{
		closedir(dp);
		auto sub_folder = awgn_codes_dir + "/" + str_m;

		if ((dp = opendir(sub_folder.c_str())) == nullptr)
		{
			static std::mutex mutex_create_folder;
			mutex_create_folder.lock();
			if ((dp = opendir(sub_folder.c_str())) == nullptr)
			{
				// mkdir mod = rwx r.x r.x
#ifdef _MSC_VER // Windows with MSVC
				if (_mkdir(sub_folder.c_str()) != 0)
				{
#elif defined(_WIN32) // MinGW on Windows
				if (mkdir(sub_folder.c_str()) != 0)
				{
#else // UNIX like
				if (mkdir(sub_folder.c_str(), S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH) != 0)
				{
#endif
					mutex_create_folder.unlock();
					std::stringstream message;
					message << "Impossible to create '" + sub_folder + "'.";
					throw runtime_error(__FILE__, __LINE__, __func__, message.str());
				}
			}
			mutex_create_folder.unlock();
		}
		else
			closedir(dp);

		auto filename = sub_folder + "/N" + str_N + "_awgn_s" + str_sigma + ".pc";

		if (!this->load_channels_file(filename, this->best_channels))
		{
#ifdef AFF3CT_POLAR_BOUNDS
			static std::mutex mutex_write_file;
			mutex_write_file.lock();
			if (!this->load_channels_file(filename, this->best_channels))
			{
				auto cmd = bin_pb_path;
				cmd += " --no-print";                                          // do not display anything
				cmd += " -q " + std::to_string(Mu);                            // quality
				cmd += " --awgn";                                              // type
				cmd += " --sigma=" + std::to_string(this->noise->get_value()); // sigma value
				cmd += " --log-length=" + str_m;                               // m
				cmd += " -f=" + filename;                                      // filename

				if (system(cmd.c_str()) == 0)
				{
					if (!this->load_channels_file(filename, this->best_channels))
					{
						mutex_write_file.unlock();
						std::stringstream message;
						message << "Can't open '" << filename << "' file.";
						throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
					}
				}
				else
				{
					mutex_write_file.unlock();
					std::stringstream message;
					message << "The following command failed: '" << cmd << "'.";
					throw runtime_error(__FILE__, __LINE__, __func__, message.str());
				}
			}
			mutex_write_file.unlock();
#else
			std::stringstream message;
			message << "Can't open '" << filename <<"' file.";
			throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
#endif
		}
	}
}

void Frozenbits_generator_TV
::check_noise()
{
	Frozenbits_generator::check_noise();

	this->noise->is_of_type_throw(tools::Noise_type::SIGMA);
}