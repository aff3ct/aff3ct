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

template <typename B>
Frozenbits_generator_file<B>
::Frozenbits_generator_file(const int K, const int N, const std::string filename)
: Frozenbits_generator<B>(K, N, 0.f), filename(filename)
{
}

template <typename B>
Frozenbits_generator_file<B>
::Frozenbits_generator_file(const int K, const int N, const float sigma)
: Frozenbits_generator<B>(K, N, sigma), filename("")
{
}

template <typename B>
Frozenbits_generator_file<B>
::~Frozenbits_generator_file()
{
}

template <typename B>
void Frozenbits_generator_file<B>
::evaluate()
{
	if(!load_channels_file(filename))
		throw invalid_argument(__FILE__, __LINE__, __func__, "'" + filename + "' file does not exist.");
}

template <typename B>
bool Frozenbits_generator_file<B>
::load_channels_file(const std::string& filename)
{
	std::ifstream in_code(filename.c_str());

	if (in_code.is_open())
	{
		std::string trash;
		in_code >> trash; // N

		if (std::stoi(trash) != this->N)
		{
			std::stringstream message;
			message << "'trash' has to be equal to 'N' ('trash' = " << trash << ", 'N' = " << this->N << ").";
			throw runtime_error(__FILE__, __LINE__, __func__, message.str());
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

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::tools::Frozenbits_generator_file<B_8>;
template class aff3ct::tools::Frozenbits_generator_file<B_16>;
template class aff3ct::tools::Frozenbits_generator_file<B_32>;
template class aff3ct::tools::Frozenbits_generator_file<B_64>;
#else
template class aff3ct::tools::Frozenbits_generator_file<B>;
#ifndef PREC_32_BIT
template class aff3ct::tools::Frozenbits_generator_file<B_32>;
#endif
#endif
// ==================================================================================== explicit template instantiation
