#ifdef _MSC_VER
#include <direct.h>
#else
#include <sys/stat.h>
#endif
#include <dirent.h>
#include <errno.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cmath>

#include "Tools/Exception/exception.hpp"
#include "Tools/Display/bash_tools.h"

#include "Frozenbits_generator_TV.hpp"

using namespace aff3ct::tools;

template <typename B>
const int Frozenbits_generator_TV<B>::Mu = 100;

template <typename B>
Frozenbits_generator_TV<B>
::Frozenbits_generator_TV(const int K, const int N,
                          const std::string awgn_codes_dir,
                          const std::string bin_pb_path,
                          const float sigma)
: Frozenbits_generator_file<B>(K, N, sigma), m((int)std::log2(N)), awgn_codes_dir(awgn_codes_dir), bin_pb_path(bin_pb_path)
{
}

template <typename B>
Frozenbits_generator_TV<B>
::~Frozenbits_generator_TV()
{
}

template <typename B>
void Frozenbits_generator_TV<B>
::evaluate()
{
	std::ostringstream s_stream;
	s_stream << std::setiosflags(std::ios::fixed) << std::setprecision(3) << this->sigma;
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
			int ret;
			// mkdir mod = rwx r.x r.x
#ifdef _MSC_VER // Windows with MSVC
			if ((ret = _mkdir(sub_folder.c_str())) != 0)
			{
#elif defined(_WIN32) // MinGW on Windows
			if((ret = mkdir(sub_folder.c_str())) != 0)
			{
#else // UNIX like
			if ((ret = mkdir(sub_folder.c_str(), S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)) != 0)
			{
#endif
				std::stringstream message;
				message << "Impossible to create '" + sub_folder + "'.";
				throw runtime_error(__FILE__, __LINE__, __func__, message.str());
			}
		}
		else
			closedir(dp);

		auto filename = sub_folder + "/N" + str_N + "_awgn_s" + str_sigma + ".pc";

		if (!this->load_channels_file(filename))
		{
#ifdef ENABLE_POLAR_BOUNDS
			auto cmd  = bin_pb_path;
			cmd      += " --no-print";                             // do not display anything
			cmd      += " -q " + std::to_string(Mu);               // quality
			cmd      += " --awgn";                                 // type
			cmd      += " --sigma=" + std::to_string(this->sigma); // sigma value
			cmd      += " --log-length=" + str_m;                  // m
			cmd      += " -f=" + filename;                         // filename

			std::clog << bold_blue("(II) Generating best channels positions file (\"")
			          << bold_blue(filename)
			          << bold_blue("\")...") << "\r";
			fflush(stdout);

			if (system(cmd.c_str()) == 0)
			{
				if (!this->load_channels_file(filename))
				{
					std::stringstream message;
					message << "Can't open '" << filename << "' file.";
					throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
				}
			}
			else
			{
				std::stringstream message;
				message << "The following command failed: '" << cmd << "'.";
				throw runtime_error(__FILE__, __LINE__, __func__, message.str());
			}
#else
			std::stringstream message;
			message << "Can't open '" << filename <<"' file.";
			throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
#endif
		}
	}
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::tools::Frozenbits_generator_TV<B_8>;
template class aff3ct::tools::Frozenbits_generator_TV<B_16>;
template class aff3ct::tools::Frozenbits_generator_TV<B_32>;
template class aff3ct::tools::Frozenbits_generator_TV<B_64>;
#else
template class aff3ct::tools::Frozenbits_generator_TV<B>;
#ifndef PREC_32_BIT
template class aff3ct::tools::Frozenbits_generator_TV<B_32>;
#endif
#endif
// ==================================================================================== explicit template instantiation
