#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

#include "Tools/Exception/exception.hpp"

#include "Frozenbits_generator_file.hpp"

using namespace aff3ct::tools;

Frozenbits_generator_file
::Frozenbits_generator_file(const int K, const int N, const std::string& filename)
: Frozenbits_generator(K, N, 0.f), filename(filename)
{
}

Frozenbits_generator_file
::Frozenbits_generator_file(const int K, const int N, const float sigma)
: Frozenbits_generator(K, N, sigma), filename("")
{
}

void Frozenbits_generator_file
::evaluate()
{
	if(!load_channels_file(filename))
		throw invalid_argument(__FILE__, __LINE__, __func__, "'" + filename + "' file does not exist.");
}

bool Frozenbits_generator_file
::load_channels_file(const std::string& filename)
{
	std::ifstream in_code(filename.c_str());

	if (in_code.is_open())
	{
		std::string trash;
		in_code >> trash; // N

		try
		{
			if (std::stoi(trash) != this->N)
			{
				std::stringstream message;
				message << "'trash' has to be equal to 'N' ('trash' = " << trash << ", 'N' = " << this->N << ").";
				throw runtime_error(__FILE__, __LINE__, __func__, message.str());
			}
		}
		catch(std::exception&)
		{
			return false;
		}

		in_code >> trash; // type
		in_code >> trash; // sigma

		for (unsigned i = 0; i < this->best_channels.size(); i++)
			in_code >> this->best_channels[i];

		in_code.close();
		return true;
	}
	else
		return false;
}
